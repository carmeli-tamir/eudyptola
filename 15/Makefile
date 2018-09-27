x86_64_target=call_eu_syscall64
x86_64_output=output64.txt

x86_64_32_target=call_eu_syscall64_32
x86_64_32_output=output64_32.txt

i386_target=call_eu_syscall32
i386_output=output32.txt

all: call_eu_syscall.c
	@gcc -o $(x86_64_target) call_eu_syscall.c
test: all
	@gcc -o $(x86_64_32_target) call_eu_syscall.c
	@gcc -o $(i386_target) call_eu_syscall.c
	
	@./$(x86_64_target) > $(x86_64_output)
	@diff $(x86_64_output) test/result
	@./$(x86_64_32_target) > $(x86_64_32_output)
	@diff $(x86_64_32_output) test/result
	@./$(x86_64_target) > $(i386_output)
	@diff $(i386_output) test/result

	@echo "ALL PASSED :)"
	
clean: 
	rm -f $(x86_64_target) $(x86_64_32_target) $(i386_target) $(x86_64_output) $(x86_64_32_output) $(i386_output)
	
