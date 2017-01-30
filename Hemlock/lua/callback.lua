io.write("Callback test...\n");
local value = print_message("This", "is", "a", "sequence", "of", "arguments!");
io.write(string.format("Callback returned: %s!\n", tostring(value)));