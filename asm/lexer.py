
COMMANDS = [
    "mov",
    "add",
    "cmp",
    "jz",
    "jnz",
    "je",
    "jmp",
    "out"
]

REGISTERS = [
    "r1",
    "r2",
    "r3"
]

def classify(element):
    if element in COMMANDS:
        return "cmd"
    elif element in REGISTERS:
        return "reg"
    elif element.isdigit():
        return "num"
    else:
        return "unknown"

if __name__ == "__main__":
    test_str = "mov r1 1"
    test_str_2 = "mov r2 r1"

    instruction_elements = test_str_2.split()

    for elem in instruction_elements:
        classification = classify(elem)
        print(f"Element: {elem}, Type: {classification}")
