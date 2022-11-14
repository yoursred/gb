import json

__ = lambda _: 2 if 'operand2' in _ else (1 if 'operand1' in _ else 0)

op = lambda _: 'R.' + _.lower() if _ in ('A', 'B', 'C', 'D', 'E', 'H', 'L') else _ if _.isnumeric() else 'mem_at(R.hl)'

def template(opc, d: dict):
    # if d is None:
    #     return '{0, 0}'
    # return '{' + f'{d["length"]}, {__(d)}' + '}'
    if d is None:
        return ''
    if __(d) == 2:
        return f'        // case ({opc}): CPU::{d["mnemonic"]}({op(d["operand1"])}, {op(d["operand2"])}); break;'
    else:
        return f'        // case ({opc}): CPU::{d["mnemonic"]}({op(d["operand1"])}); break;'


with open('opcodes.json', 'rb') as fp:
    opcodes = json.load(fp)


arr = ''

for i in range(256):
    opc = '0x{:02x}'.format(i)
    arr += template(opc, opcodes['cbprefixed'].get(opc))
    if i != 255 and opc in opcodes['cbprefixed']:
        arr += '\n'
        # if ((i + 1) % 8) == 0:
        #     arr += '\n'

print(arr)