This version of the program recursively lists all the files in a folder, and as soon as it encounters a directory, it abandons the previous directory and starts enumerating the content of the new dir.
This implementation is fairly simple, but it's sort of annoying that it often breaks working in the current directory and passes over the new one, and eventually it goes back to the previous one.
