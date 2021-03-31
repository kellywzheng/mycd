import sys
import re

'''
Reduce multiple consecutive forward-slashes ("/") to one forward-slash
Remove any sequence that consists of a file name component (other than "." or ".."), 
followed by "/", followed by the file name component ".."
'''
def simplify(directory):
    while True:
        old_dir = directory
        directory = re.sub("//+", '/', directory)                       # replace multiple "/" to a single slash

        idx = directory.find('/../')                                    # find the first occurence of "/../"
        if idx > 0:
            prev_idx = directory.rfind('/', 0, idx)                     # find the last occurence of "/" in the range 0 to idx
            idx = idx + 4                                               # increment idx to now be at the start of the next file name component
            directory = directory[0:prev_idx + 1] + directory[idx: ]    # remove the "/../" sequence

        if directory == old_dir:                                        # break out of loop when the above conditions no longer apply
            break
            
    return directory

'''
Given the current directory path and a new directory path, 
return either a new path or an error message
'''
def get_new_path(curr_dir, new_dir):        
    curr_dir_stack = []
    
    curr_dir_split = curr_dir.split("/")                                # Split the current directory by "/" and add to stack
    for split in curr_dir_split:
        if split:
            curr_dir_stack.append(split)
            
    new_dir_split = new_dir.replace("/", "!/!").split("!")              # Keep "/" in the new path when splitting
    new_dir_split = [x for x in new_dir_split if x != ""]

    if (                                                                # Check if the new directory path is valid 
        not new_dir_split[0].isalnum() 
        and new_dir_split[0] != "/" 
        and new_dir_split[0] != "." 
        and new_dir_split[0] != ".."
       ):
        return(new_dir + ":" + " No such file or directory")            
        
    if new_dir_split[0] == "/":                                         # If the new directory path contains a leading "/"
        new_dir_stack = []
        for split in new_dir_split:
            if split and split.isalnum():                               # Fill stack according to the path in new_dir
                new_dir_stack.append(split)
                new_dir_split.remove(split)
        stack = process_stack(new_dir_stack, new_dir_split)
    else:
        stack = process_stack(curr_dir_stack, new_dir_split)

    result = ""                                                         # Create a path based on the stack
    if not stack:
        result = "/"
    else:  
        for section in stack:
            if section:
                result += "/"
                result += section
    
    return(result)

'''
Given a stack and a list of a split directory path, 
update the stack based on the commands seen in the list
'''
def process_stack(stack, split_list):                                   
    for i in range(0, len(split_list)):
        if split_list[i] == "..":
            if stack:
                stack.pop()
        elif split_list[i] == "." or split_list[i] == "/":
            continue
        else:
            stack.append(split_list[i])

    return stack

def run_tests():
    pass
    
def main():
    if len(sys.argv) == 1:
        run_tests()
        return

    curr_directory = sys.argv[1]
    new_directory = sys.argv[2]
    new_directory = simplify(new_directory)
    print(get_new_path(curr_directory, new_directory))
    
       
if __name__ == "__main__":
    main()