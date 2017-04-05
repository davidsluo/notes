# Brainstorm
David Luo with Stone Daniel and Sophia Mapau
811357331

For this lab, the code will be something like this:
```
function copy(source, target) {
    if item is directory {
        if recursive {
            for each item in source {
                if item is not ".." or "." {
                    copy(item) // this function
                    cp(item) // system call
                }
            }
        }
        else return -1;
    }
    else cp(item);
}
```
