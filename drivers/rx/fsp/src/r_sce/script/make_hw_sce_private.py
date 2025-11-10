import os
import re

# Simple regex for C function definitions (single-line only, not static or macros)
FUNC_DEF_REGEX = re.compile(
    r'''
    ^\s*                                  # Leading whitespace
    (?:[a-zA-Z_][\w\s\*\(\)]*)            # Return type
    \s+                                   # Whitespace
    ([a-zA-Z_]\w*)                        # Function name
    \s*                                   # Optional space
    \(([^)]*)\)                           # Function arguments
    \s*                                   # Optional space
    \{                                    # Opening brace (start of body)
    ''', re.MULTILINE | re.VERBOSE
)

def extract_prototypes_from_c_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    matches = FUNC_DEF_REGEX.findall(content)
    prototypes = []

    for name, args in matches:
        # Guess return type from previous line or context
        before_func = re.search(r'([a-zA-Z_][\w\s\*\(\)]*)\s+' + name + r'\s*\(', content)
        return_type = before_func.group(1).strip() if before_func else 'void'
        prototype = f"{return_type} {name}({args.strip()});"
        prototypes.append(prototype)

    return prototypes

def collect_all_c_files(root_dir):
    c_files = []
    for dirpath, _, filenames in os.walk(root_dir):
        for fname in filenames:
            if fname.endswith('.c'):
                c_files.append(os.path.join(dirpath, fname))
    return c_files

def generate_prototype_file(output_file, root_dir='.'):
    all_prototypes = []
    c_files = collect_all_c_files(os.path.join(root_dir, 'adaptors'))
    c_files += collect_all_c_files(os.path.join(root_dir, 'primitive'))
    for cfile in c_files:
        prototypes = extract_prototypes_from_c_file(cfile)
        if prototypes:
            all_prototypes.append(f"// From {os.path.relpath(cfile, root_dir)}")
            all_prototypes.extend(prototypes)
            all_prototypes.append('')

    with open(output_file, 'w') as f:
        f.write("// Auto-generated header of function prototypes\n\n")
        for prototype in all_prototypes:
            f.write(prototype + '\n')

    print(f"Prototypes written to: {output_file}")

# Example usage
if __name__ == '__main__':
    generate_prototype_file("my_hw_sce_rx_private.h", root_dir="rx/fsp/src/r_sce/crypto_procedures/src/rsip_e50d/plainkey/")
