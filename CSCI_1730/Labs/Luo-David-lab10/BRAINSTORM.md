# Brainstorm
David Luo with Stone Daniel and Sophia Mapau

For this lab, the code will be something like this:
```
function remove(directory=".") {
    for each item in directory {
        if item is directory and item is not ".." {
            remove(item)
        }
        unlink(item)
    }
}
```
