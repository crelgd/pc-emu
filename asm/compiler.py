
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

            try:
                self.process_instruction(in_str)
            except IndexError as e:
                print(f"Error processing instruction: {line.strip()}")
                print(f"Error message: {str(e)}")

        return self.compiler_out

    def process_instruction(self, in_str):
        instruction = in_str[0].lower()

        if instruction == "mov":
            dest = in_str[1].replace(',', '')
            src = in_str[2] if len(in_str) > 2 else None
            self.handle_mov(dest, src)
        elif instruction == "add":
            dest = in_str[1].replace(',', '')
            src = in_str[2] if len(in_str) > 2 else None
            self.handle_add(dest, src)
        elif instruction == "cmp":
            dest = in_str[1].replace(',', '')
            src = in_str[2] if len(in_str) > 2 else None
            self.handle_cmp(dest, src)
        elif instruction == "jmp":
            address = in_str[1]
            self.handle_jmp(address)
        elif instruction == "jiz":
            address = in_str[1]
            self.handle_jiz(address)
        elif instruction == "jinz":
            address = in_str[1]
            self.handle_jinz(address)
        elif instruction == "jie":
            address = in_str[1]
            self.handle_jie(address)
        elif instruction == "out":
            port = in_str[1]
            reg = in_str[2]
            self.handle_out(port, reg)
        elif instruction == "stop":
            self.handle_STOP()
        elif instruction == "call":
            address = in_str[1]
            self.handle_call(address)
        elif instruction == "ret":
            self.handle_ret()
        elif instruction == "push":
            value = in_str[1]
            self.handle_stack_push(value)
        elif instruction == "pop":
            value = in_str[1]
            self.handle_stack_pop(value)
        elif instruction == "byte":
            value = in_str[1:]
            self.handle_byte_add(value)
        elif instruction.startswith(";"):
            pass 
        else:
            print(f"Unknown instruction: {instruction}")

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
        if address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C4"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_jmp_register_codes(address)
            self.compiler_out.extend(code.split())

    def handle_jiz(self, address):
        if address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C0"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_jiz_register_codes(address)
            self.compiler_out.extend(code.split())

    def handle_jinz(self, address):
        if address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C1"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_jinz_register_codes(address)
            self.compiler_out.extend(code.split())

    def handle_jie(self, address):
        if address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C2"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_jie_register_codes(address)
            self.compiler_out.extend(code.split())

    def handle_out(self, port, reg):
        code = self.get_out_code(reg)
        value = port[2:] if port.startswith('0x') else f"{int(port):02X}"
        self.compiler_out.extend([code, value])

    def handle_STOP(self):
        code = "FF"
        self.compiler_out.append(code)

    def handle_call(self, address):
        if address.startswith("[") and address.endswith("]"):
            address = address.replace("[", "").replace("]", "")
            code = "8D"
        else:
            code = "83"
            
        if address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        else:
            code = self.get_call_code(address)
            self.compiler_out.extend([code])

    def handle_ret(self):
        code = "A1 00"
        self.compiler_out.extend([code])

    def handle_stack_push(self, value):
        if value.startswith("[") and value.endswith("]"):
            value = value.replace("[", "").replace("]", "")

        if value.isdigit() or (value.startswith('0x') and self.is_hex(value)):
            code = "8A"
            val = value[2:] if value.startswith('0x') else f"{int(value):02X}"
            self.compiler_out.extend([code, val])
        else:
            code = self.get_stack_push_code(value)
            self.compiler_out.extend([code])

    def handle_stack_pop(self, value):
        if value.startswith("[") and value.endswith("]"):
            value = value.replace("[", "").replace("]", "")

        if value.isdigit() or (value.startswith('0x') and self.is_hex(value)):
            code = "7A"
            val = value[2:] if value.startswith('0x') else f"{int(value):02X}"
            self.compiler_out.extend([code, val])
        else:
            code = self.get_stack_pop_code(value)
            self.compiler_out.extend([code])

    def handle_byte_add(self, value):
        for i in range(len(value)):
            #print(value[i])
            if (value[i].startswith("0x")):
                val = value[i][2:]
            else: 
                val = f"{int(val):02X}"
            self.compiler_out.append(val)

    def is_hex(self, s):
        try:
            int(s, 16)
            return True
        except ValueError:
            return False

    def get_call_code(self, address):
        out = {
            "r1": "86 10",
            "r2": "86 11",
            "r3": "86 12"
        }
        return out.get(address, "UNKNOWN")

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

    def get_jmp_register_codes(self, dest):
        out = {
            "r1": "C6 10",
            "r2": "C6 11",
            "r3": "C6 12"
        } 
        return out.get(dest, "UNKNOWN")

    def get_jiz_register_codes(self, dest):
        out = {
            "r1": "C7 10",
            "r2": "C7 11",
            "r3": "C7 12"
        }
        return out.get(dest, "UNKNOWN")

    def get_jinz_register_codes(self, dest):
        out = {
            "r1": "C8 10",
            "r2": "C8 11",
            "r3": "C8 12"
        }
        return out.get(dest, "UNKNOWN")

    def get_jie_register_codes(self, dest):
        out = {
            "r1": "C9 10",
            "r2": "C9 11",
            "r3": "C9 12"
        }
        return out.get(dest, "UNKNOWN")

    def get_stack_push_code(self, value):
        out = {
            "r1": "2A 10",
            "r2": "2A 11",
            "r3": "2A 12"
        }
        return out.get(value, "UNKNOWN")

    def get_stack_pop_code(self, value):
        out = {
            "r1": "5A 10",
            "r2": "5A 11",
            "r3": "5A 12"
        }
        return out.get(value, "UNKNOWN")
    
    def write_to_output(self, file_name):
        with open(file_name, "wb") as file:
            for hex_str in self.compiler_out:
                file.write(bytes.fromhex(hex_str))
            file.write(bytes.fromhex("4D"))
