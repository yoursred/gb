import json

__ = lambda _: 2 if 'operand2' in _ else (1 if 'operand1' in _ else 0)

def template(opc, d: dict):
    # if d is None:
    #     return '{0, 0}'
    # return '{' + f'{d["length"]}, {__(d)}' + '}'
    if d is None:
        return ''
    return f'        // case ({opc}): Gameboy::{d["mnemonic"]}();break;'


with open('opcodes.json', 'rb') as fp:
    opcodes = json.load(fp)


arr = ''

for i in range(256):
    opc = '0x{:02x}'.format(i)
    arr += template(opc, opcodes['unprefixed'].get(opc))
    if i != 255 and opc in opcodes['unprefixed']:
        arr += '\n'
        # if ((i + 1) % 8) == 0:
        #     arr += '\n'

print(arr)