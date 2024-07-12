from lexer import classify

DEBUG = False

class Compiler:
    def __init__(self, file_name):
        self.file_name = file_name
        self.lines = None
        self.compiler_out = []

    def read_file(self):
        with open(self.file_name, "r") as file:
            self.lines = file.readlines()

    def comp(self):
        self.read_file()
        for line in self.lines:
            in_str = line.strip().split()
            if not in_str:
                continue
            for elem in in_str:
                classification = classify(elem)
                if DEBUG:
                    print(f"Instruction: {elem}, {classification}")

            self.process_instruction(in_str)

        return self.compiler_out

    def process_instruction(self, in_str):
        instruction = in_str[0].lower()
        dest = in_str[1].replace(',', '')
        src = in_str[2] if len(in_str) > 2 else None

        if instruction == "mov":
            self.handle_mov(dest, src)
        elif instruction == "add":
            self.handle_add(dest, src)
        elif instruction == "cmp":
            self.handle_cmp(dest, src)
        elif instruction == "jmp":
            self.handle_jmp(dest)
        elif instruction == "jiz":
            self.handle_jiz(dest)
        elif instruction == "jinz":
            self.handle_jinz(dest)
        elif instruction == "jie":
            self.handle_jie(dest)
        elif instruction == "out":
            self.handle_out(dest, src)

    def handle_mov(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_mov_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_mov_register_code(dest, src)
            self.compiler_out.extend(code.split())

    def handle_add(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_add_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_add_register_code(dest, src)
            self.compiler_out.extend(code.split())

    def handle_cmp(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_cmp_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_cmp_register_code(dest, src)
            self.compiler_out.extend(code.split())

    def handle_jmp(self, address):
        code = "C4"
        value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
        self.compiler_out.extend([code, value])

    def handle_jiz(self, address):
        code = "C0"
        value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
        self.compiler_out.extend([code, value])

    def handle_jinz(self, address):
        code = "C1"
        value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
        self.compiler_out.extend([code, value])

    def handle_jie(self, address):
        code = "C2"
        value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
        self.compiler_out.extend([code, value])

    def handle_out(self, port, reg):
        code = self.get_out_code(reg)
        value = port[2:] if port.startswith('0x') else f"{int(port):02X}"
        self.compiler_out.extend([code, value])

    def is_hex(self, s):
        try:
            int(s, 16)
            return True
        except ValueError:
            return False

    def get_mov_code(self, register):
        mov_codes = {
            "r1": "E1",
            "r2": "E2",
            "r3": "E3"
        }
        return mov_codes.get(register, "UNKNOWN")

    def get_mov_register_code(self, dest, src):
        mov_register_codes = {
            ("r1", "r2"): "E0 10",
            ("r1", "r3"): "E0 11",
            ("r2", "r1"): "E0 12",
            ("r2", "r3"): "E0 13",
            ("r3", "r1"): "E0 14",
            ("r3", "r2"): "E0 15"
        }
        return mov_register_codes.get((dest, src), "UNKNOWN")

    def get_add_code(self, register):
        add_codes = {
            "r1": "B2",
            "r2": "B3",
            "r3": "B4"
        }
        return add_codes.get(register, "UNKNOWN")

    def get_add_register_code(self, dest, src):
        add_register_codes = {
            ("r1", "r2"): "B7 10",
            ("r1", "r3"): "B7 11",
            ("r2", "r1"): "B7 12",
            ("r2", "r3"): "B7 13",
            ("r3", "r1"): "B7 14",
            ("r3", "r2"): "B7 15"
        }
        return add_register_codes.get((dest, src), "UNKNOWN")

    def get_cmp_code(self, register):
        cmp_codes = {
            "r1": "F1",
            "r2": "F2",
            "r3": "F3"
        }
        return cmp_codes.get(register, "UNKNOWN")

    def get_cmp_register_code(self, dest, src):
        cmp_register_codes = {
            ("r1", "r2"): "F0 10",
            ("r1", "r3"): "F0 11",
            ("r2", "r1"): "F0 12",
            ("r2", "r3"): "F0 13",
            ("r3", "r1"): "F0 14",
            ("r3", "r2"): "F0 15"
        }
        return cmp_register_codes.get((dest, src), "UNKNOWN")

    def get_out_code(self, register):
        out_codes = {
            "r1": "D1",
            "r2": "D2",
            "r3": "D3"
        }
        return out_codes.get(register, "UNKNOWN")

    
    def write_to_output(self, file_name):
        with open(file_name, "wb") as file:
            for hex_str in self.compiler_out:
                file.write(bytes.fromhex(hex_str))
