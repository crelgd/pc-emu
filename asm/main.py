import sys
import compiler as cp

def main():

    if len(sys.argv) < 2:
        print("Usage: \n<file_name> <output_name>")
        return 

    file_name = sys.argv[1]
    output_name = sys.argv[2]

    compiler = cp.Compiler(file_name)
    compiler.comp()
    compiler.write_to_output(output_name)

if __name__ == "__main__":
    main()