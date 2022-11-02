import json

__ = lambda _: 2 if 'operand2' in _ else (1 if 'operand1' in _ else 0)

def template(d: dict):
    if d is None:
        return '{0, 0}'
    return '{' + f'{d["length"]}, {__(d)}' + '}'


with open('opcodes.json', 'rb') as fp:
    opcodes = json.load(fp)


arr = ''

for i in range(256):
    arr += template(opcodes['unprefixed'].get('0x{:02x}'.format(i)))
    if i != 255:
        arr += ', '
        if ((i + 1) % 8) == 0:
            arr += '\n'

print('byte opcodes[][2] = {\n' + arr + '\n}')