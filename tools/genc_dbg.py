import json

__ = lambda _: 2 if 'operand2' in _ else (1 if 'operand1' in _ else 0)

def template(opc: str, d: dict):
    # if d is None:
    #     return '{0, 0}'
    # return '{' + f'{d["length"]}, {__(d)}' + '}'
    if d is None:
        return '"NUL"'
    i = __(d)
    return f'"{d["mnemonic"]}' + (f' {d["operand1"]}, {d["operand2"]}"' if i == 2 else (f' {d["operand1"]}"' if i == 1 else '"'))


with open('opcodes.json', 'rb') as fp:
    opcodes = json.load(fp)


arr = 'const char* unprefixed[] = {\n    '

for i in range(256):
    opc = '0x{:02x}'.format(i)
    s = template(opc, opcodes['unprefixed'].get(opc))
    arr += s
    arr += ', '
    if ((i + 1  ) % 8) == 0:
        arr += '\n    '

for i in range(256):
    opc = '0x{:02x}'.format(i)
    s = template(opc, opcodes['cbprefixed'].get(opc))
    arr += s
    if i != 255:
        arr += ', '
        if ((i + 1  ) % 8) == 0:
            arr += '\n    '
arr += '\n}'
arr = arr.replace('(', '[').replace(')', ']').replace('r8', 'e8')
print(arr)