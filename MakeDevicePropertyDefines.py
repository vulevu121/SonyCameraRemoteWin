import re

def main():
    with open('app/CRSDK/CrDeviceProperty.h', mode='rt') as f:
        lines = f.readlines()

        map_property_codes = []
        map_property_names = []

        for line in lines:
            if 'enum CrDevicePropertyCode' in line or '{' in line or 'reserved' in line:
                continue

            if '};' in line:
                break
            
            if 'CrDeviceProperty_' in line:
                m = re.search(r'(CrDeviceProperty_([A-Za-z0-9_]+))[\t ,=]', line)
                map_property_codes.append('{{TEXT("{}"), {}}}'.format(m.group(2), m.group(1)))
                map_property_names.append('{{{}, TEXT("{}")}}'.format(m.group(1), m.group(2)))
            
        print(',\n'.join(map_property_codes))
        
        print(',\n'.join(map_property_names))

if __name__ == '__main__':
    main()