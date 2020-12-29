// every compilation should include this file

// ensure that main is in its own section so that it can be relocated freely
void main() __attribute__((section(".text.main")));
