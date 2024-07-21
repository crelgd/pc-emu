import sys
import compiler as cp

### Main func
def main():
    if len(sys.argv) < 2: ## if havent arguments, returns
        print("Usage: \n<file_name> <output_name>")
        return 

    file_name = sys.argv[1] ## asm file
    output_name = sys.argv[2] ## bin file

    compiler = cp.Compiler(file_name)
    compiler.register_address()
    compiler.comp()

    compiler.write_to_output(output_name)

if __name__ == "__main__":
    main()
