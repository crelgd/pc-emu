import binascii
import json

class Compiler:
    def __init__(self, file_name):
        self.file_name = file_name
        self.lines = None
        self.compiler_out = []

        self.program_count = 0
        self.program_data = "[]"
        self.program_data = json.loads(self.program_data)

        self.registers = ["r1", "r2", "r3"]

    def read_file(self):
        with open(self.file_name, "r") as file:
            self.lines = file.readlines()

    # selects addresses for labels /
    def register_address(self):
        self.read_file() ## read file
        for line in self.lines:
            in_str = line.strip().split() ## split str to array
            if not line:
                continue 
            try:
                self.process_register(in_str)
            except IndexError as e:
                pass

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

    ### checks instructions
    def process_register(self, in_str):
        instruction = in_str[0]
        if instruction in ["mov", "add", "sub", "div", "mul",
            "cmp", "jmp", 
            "jiz", "jinz", "jie", "jine", "out", "call",
            "ret", "push", "pop"]: ## checks instructions which haves 2 bytes
            self.program_count_add(2)
        elif instruction in ["stop"]: ## checks emu marks which haves 1 byte
            self.program_count_add(1)
        elif instruction.startswith(";"): ## checks comments
            pass 
        elif instruction == "byte": ## checks set of bytes
            self.program_count_add(len(in_str[1:]))
        elif instruction.startswith("!"):
            label = instruction[1:] ## label name
            self.program_count_save(label)
        else:
            print(f"Unknown instruction: {instruction}")

    def process_instruction(self, in_str):
        instruction = in_str[0]

        if instruction == "mov":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_mov(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "add":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_add(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "sub":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_sub(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "mul":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_mul(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "div":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_div(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "cmp":
            try:
                dest = in_str[1].replace(',', '')
                src = in_str[2] if len(in_str) > 2 else None
                self.handle_cmp(dest, src)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "jmp":
            try:
                address = in_str[1]
                self.handle_jmp(address)
            except:
                print("ERROR: Unknown error")
        elif instruction == "jiz":
            try:
                address = in_str[1]
                self.handle_jiz(address)
            except:
                print("ERROR: Unknown error")
        elif instruction == "jinz":
            try:
                address = in_str[1]
                self.handle_jinz(address)
            except:
                print("ERROR: Unknown error")
        elif instruction == "jie":
            try:
                address = in_str[1]
                self.handle_jiz(address)
            except:
                print("ERROR: Unknown error")
        elif instruction == "out":
            try:
                port = in_str[1].replace("," , "")
                reg = in_str[2]
                self.handle_out(port, reg)
            except AttributeError as e:
                print(f"ERROR: {e}\nWrite like this:\n<instruction> <register>, <value/register>")
        elif instruction == "stop":
            try:
                self.handle_STOP()
            except:
                print("ERROR: Unknown error")
        elif instruction == "call":
            try:
                address = in_str[1]
                self.handle_call(address)
            except:
                print("ERROR: Unknown error")
        elif instruction == "ret":
            try:
                self.handle_ret()
            except:
                print("ERROR: Unknown error")
        elif instruction == "push":
            try:
                value = in_str[1]
                self.handle_stack_push(value)
            except:
                print("ERROR: Unknown error")
        elif instruction == "pop":
            try:
                value = in_str[1]
                self.handle_stack_pop(value)
            except:
                print("ERROR: Unknown error")
        elif instruction == "byte":
            try:
                value = in_str[1:]
                self.handle_byte_add(value)
            except:
                print("ERROR: Unknown error")
        elif instruction.startswith(";"):
            pass 
        elif instruction.startswith("!"):
            pass

    def program_count_add(self, count):
        self.program_count += count

    def program_count_save(self, label):
        address = format(self.program_count, '02X')
        new_data = {"label": label, "address": address}
        self.program_data.append(new_data)
        json.dumps(self.program_data, ensure_ascii=False, indent=4)

    def program_data_search_address(self, label):
        for item in self.program_data:
            if item.get("label") == label:
                return item.get("address")
        return None

    def handle_mov(self, dest, src):
        src_label_data = self.program_data_search_address(src)

        if src_label_data:
            value = src_label_data
            code = self.get_mov_code(dest)
            self.compiler_out.extend([code, value])
        # I dont know how best to release this :/
        elif dest.startswith('$'): # check value change
            if src not in self.registers: # if not register
                print("ERROR: Incorrect spelling\n\
                     -\tmov <address> <only_register>")
            dest = dest[1:] # del $
            dest_label_data = self.program_data_search_address(dest) # searching label
            if dest.startswith('0x'):
                dest = dest[2:] # del 0x
            elif dest_label_data: # if label
                dest = dest_label_data
            else: dest = f"{int(dest):02X}" # to hex

            code = self.get_mov_address_code(src)
            self.compiler_out.extend([code, dest]) # write to array

        elif src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_mov_code(dest)
            value = src[2:].upper() if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_mov_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        else:
            print(f"ERROR: Not found '{src}'")

    def handle_add(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_add_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_add_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        else:
            print(f"ERROR: Unknown error")

    def handle_sub(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_sub_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_sub_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        else:
            print(f"ERROR: Unknown error")

    def handle_mul(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_mul_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_mul_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        else:
            print(f"ERROR: Unknown error")

    def handle_div(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_div_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_div_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        else:
            print(f"ERROR: Unknown error")

    def handle_cmp(self, dest, src):
        if src.isdigit() or (src.startswith('0x') and self.is_hex(src)):
            code = self.get_cmp_code(dest)
            value = src[2:] if src.startswith('0x') else f"{int(src):02X}"
            self.compiler_out.extend([code, value])
        elif src in self.registers:
            if dest in self.registers:
                code = self.get_cmp_register_code(dest, src)    
                self.compiler_out.extend(code.split())
            else:
                print(f"ERROR: Unknown register '{dest}'")
        elif src not in self.registers:
            print(f"ERROR: Unknown register '{src}'")
        else:
            print(f"ERROR: Unknown error")

    def handle_jmp(self, address):
        address_label = self.program_data_search_address(address)
        if address_label:
            code = "C4"
            self.compiler_out.extend([code, address_label])
        elif address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C4"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        elif address in self.registers:
            code = self.get_jmp_register_codes(address)
            self.compiler_out.extend(code.split())
        elif address not in self.registers:
            print(f"ERROR: Unknown register '{address}'")
        else:
            print("ERROR: Unknown error")

    def handle_jiz(self, address):
        address_label = self.program_data_search_address(address)
        if address_label:
            code = "C0"
            self.compiler_out.extend([code, address_label])
        elif address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C0"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        elif address in self.registers:
            code = self.get_jiz_register_codes(address)
            self.compiler_out.extend(code.split())
        elif address not in self.registers:
            print(f"ERROR: Unknown register '{address}'")
        else:
            print("ERROR: Unknown error")

    def handle_jinz(self, address):
        address_label = self.program_data_search_address(address)
        if address_label:
            code = "C1"
            self.compiler_out.extend([code, address_label])
        elif address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            code = "C1"
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        elif address in self.registers:
            code = self.get_jinz_register_codes(address)
            self.compiler_out.extend(code.split())
        elif address not in self.registers:
            print(f"ERROR: Unknown register '{address}'")
        else:
            print("ERROR: Unknown error")

    def handle_out(self, port, reg):
        if reg in self.registers:
            code = self.get_out_code(reg)
            value = port[2:] if port.startswith('0x') else f"{int(port):02X}"
            self.compiler_out.extend([code, value])
        elif reg not in self.registers:
            print(f"ERROR: Unknown register: '{reg}'")
        else:
            print("ERROR: Unknown error")

    def handle_STOP(self):
        code = "FF"
        self.compiler_out.append(code)

    def handle_call(self, address):
        if address.startswith("["): # [addr
            if address.endswith("]"): #addr]
                address = address.replace("[", "").replace("]", "")
                address_label = self.program_data_search_address(address)
                code = "8D"
            else:
                print("ERROR: value not closed")
        elif address.endswith("]"): # addr]
            print("ERROR: value not opened")
        else:
            address_label = self.program_data_search_address(address)
            code = "83"
            
        if address_label:
            value = address_label
            self.compiler_out.extend([code, value])
        elif address.isdigit() or (address.startswith('0x') and self.is_hex(address)):
            value = address[2:] if address.startswith('0x') else f"{int(address):02X}"
            self.compiler_out.extend([code, value])
        elif address in self.registers:
            code = self.get_call_code(address)
            self.compiler_out.extend([code])
        elif address not in self.registers:
            print(f"ERROR: Unknown register '{address}'")
        else:
            print("ERROR: Unknown error")

    def handle_ret(self):
        code = "A1 00"
        self.compiler_out.extend([code])

    def handle_stack_push(self, value):
        if value.startswith("["): # [addr
            if value.endswith("]"): #addr]
                value = value.replace("[", "").replace("]", "")
                value_label = self.program_data_search_address(value)
                code = "8A"
            else:
                print("ERROR: value not closed")
        elif value.endswith("]"): # addr]
            print("ERROR: value not opened")
        else:
            value_label = self.program_data_search_address(value)
            code = "00"
            if value not in ["r1", "r2", "r3"]:
                print("WARNING: If the value is not [], the command code will be '00'")

        if value_label:
            val = value_label
            self.compiler_out.extend([code, val])
        elif value.isdigit() or (value.startswith('0x') and self.is_hex(value)):
            val = value[2:] if value.startswith('0x') else f"{int(value):02X}"
            self.compiler_out.extend([code, val])
        elif value in self.registers:
            code = self.get_stack_push_code(value)
            self.compiler_out.extend([code])
        elif value not in self.registers:
            print(f"ERROR: Unknown register '{value}'")
        else:
            print("ERROR: Unknown error")

    def handle_stack_pop(self, value):
        if value.startswith("["): # [addr
            if value.endswith("]"): #addr]
                value = value.replace("[", "").replace("]", "")
                value_label = self.program_data_search_address(value)
                code = "7A"
            else:
                print("ERROR: value not closed")
        elif value.endswith("]"): # addr]
            print("ERROR: value not opened")
        else:
            value_label = self.program_data_search_address(value)
            code = "00"
            if value not in self.registers:
                print("WARNING: If the value is not [], the command code will be '00'")

        if value_label:
            val = value_label
            self.compiler_out.extend([code, val])
        elif value.isdigit() or (value.startswith('0x') and self.is_hex(value)):
            val = value[2:] if value.startswith('0x') else f"{int(value):02X}"
            self.compiler_out.extend([code, val])
        elif value in self.registers:
            code = self.get_stack_pop_code(value)
            self.compiler_out.extend([code])
        elif value not in self.registers:
            print(f"ERROR: Unknown register '{value}'")
        else:
            print("ERROR: Unknown error")


    def handle_byte_add(self, value):
        val = ""
        val2 = ""
    
        for item in value:
            if item.startswith("0x"):
                val += item[2:]
            elif item.startswith(";"):
                print("ERROR: Comments cannot be left in the 'byte' statement")
                continue
            elif item.startswith('"') and item.endswith('"'):
                val += ''.join(f"{ord(c):02X}" for c in item[1:-1])
            else:
                try:
                    val += f"{int(item):02X}"
                except ValueError:
                    print(f"ERROR: Invalid value '{item}'")
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

    def get_mov_address_code(self, register):
        mov_codes = {
            "r1": "E4",
            "r2": "E5",
            "r3": "E6"
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

    def get_sub_code(self, register):
        add_codes = {
            "r1": "1E",
            "r2": "2E",
            "r3": "3E"
        }
        return add_codes.get(register, "UNKNOWN")

    def get_sub_register_code(self, dest, src):
        add_register_codes = {
            ("r1", "r2"): "0E 10",
            ("r1", "r3"): "0E 11",
            ("r2", "r1"): "0E 12",
            ("r2", "r3"): "0E 13",
            ("r3", "r1"): "0E 14",
            ("r3", "r2"): "0E 15"
        }
        return add_register_codes.get((dest, src), "UNKNOWN")

    def get_mul_code(self, register):
        out = {
            "r1": "13",
            "r2": "23",
            "r3": "33"
        }
        return out.get(register, "UNKNOWN")

    def get_mul_register_code(self, dest, src):
        out = {
            ("r1", "r2"): "73 10",
            ("r1", "r3"): "73 11",
            ("r2", "r1"): "73 12",
            ("r2", "r3"): "73 13",
            ("r3", "r1"): "73 14",
            ("r3", "r2"): "73 15"
        }
        return out.get((dest, src), "UNKNOWN")

    def get_div_code(self, register):
        out = {
            "r1": "17",
            "r2": "27",
            "r3": "37"
        }
        return out.get(register, "UNKNOWN")

    def get_div_register_code(self, dest, src):
        out = {
            ("r1", "r2"): "47 10",
            ("r1", "r3"): "47 11",
            ("r2", "r1"): "47 12",
            ("r2", "r3"): "47 13",
            ("r3", "r1"): "47 14",
            ("r3", "r2"): "47 15"
        }
        return out.get((dest, src), "UNKNOWN")

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
        try:
            with open(file_name, "wb") as file:
                for hex_str in self.compiler_out:
                    file.write(bytes.fromhex(hex_str))
                file.write(bytes.fromhex("4D"))
        except ValueError as e:
            print(f"ERROR: Cannot be written to file")
