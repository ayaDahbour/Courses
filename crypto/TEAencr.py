import numpy as np
from PIL import Image

delta = 0x9e3779b9
mask = 0xffffffff

def tea_encrypt(plain_text, key):
    L, R = plain_text
    sum = 0
    for _ in range(32):
        sum = (sum + delta) & mask
        L = (L + (((R << 4) + key[0]) ^ (R + sum) ^ ((R >> 5) + key[1]))) & mask
        R = (R + (((L << 4) + key[2]) ^ (L + sum) ^ ((L >> 5) + key[3]))) & mask
    return (L, R)

def tea_decrypt(cipher_text, key):
    L, R = cipher_text
    sum = (delta * 32) & mask
    for _ in range(32):
        R = (R - (((L << 4) + key[2]) ^ (L + sum) ^ ((L >> 5) + key[3]))) & mask
        L = (L - (((R << 4) + key[0]) ^ (R + sum) ^ ((R >> 5) + key[1]))) & mask
        sum = (sum - delta) & mask
    return (L, R)

def process_image(image_path, mode='RGB'):
    img = Image.open(image_path)
    img = img.convert(mode)
    data = np.array(img)
    flat_data = data.flatten()
    return flat_data, img.size, img.mode

def image_to_int_blocks(image_data):
    image_blocks = []
    for i in range(0, len(image_data), 2):
        if i + 1 < len(image_data):
            block = (image_data[i], image_data[i + 1])
        else:
            block = (image_data[i], 0)
        image_blocks.append(block)
    return image_blocks

def int_blocks_to_image(blocks, size, mode, file_name):
    image_data = []
    for block in blocks:
        image_data.extend(block)
    image_array = np.array(image_data, dtype=np.uint8).reshape(size[1], size[0], 3)
    img = Image.fromarray(image_array, mode)
    img.save(file_name)
    print(f"Image saved as {file_name}")

def main():
    key = [int(input(f"Enter key part {i + 1} (hex): "), 16) for i in range(4)]
    iv = (int(input("Enter IV part 1 (hex): "), 16), int(input("Enter IV part 2 (hex): "), 16))
    image_path = input("Enter the path of the image to encrypt/decrypt: ")
    image_path = r"{}".format(image_path)

    image_data, size, mode = process_image(image_path)
    image_blocks = image_to_int_blocks(image_data)

    # ECB Mode
    encrypted_ecb = [tea_encrypt(block, key) if i >= 10 else block for i, block in enumerate(image_blocks)]
    decrypted_ecb = [tea_decrypt(block, key) if i >= 10 else block for i, block in enumerate(encrypted_ecb)]
    int_blocks_to_image(encrypted_ecb, size, mode, "encrypted_ecb.png")
    int_blocks_to_image(decrypted_ecb, size, mode, "decrypted_ecb.png")

    # CBC Mode
    encrypted_cbc = image_blocks[:10]
    previous_block = iv
    for i in range(10, len(image_blocks)):
        block_to_encrypt = (image_blocks[i][0] ^ previous_block[0], image_blocks[i][1] ^ previous_block[1])
        encrypted_block = tea_encrypt(block_to_encrypt, key)
        encrypted_cbc.append(encrypted_block)
        previous_block = encrypted_block
    decrypted_cbc = image_blocks[:10]
    previous_block = iv
    for i in range(10, len(encrypted_cbc)):
        decrypted_block = tea_decrypt(encrypted_cbc[i], key)
        decrypted_block = (decrypted_block[0] ^ previous_block[0], decrypted_block[1] ^ previous_block[1])
        decrypted_cbc.append(decrypted_block)
        previous_block = encrypted_cbc[i]
    int_blocks_to_image(encrypted_cbc, size, mode, "encrypted_cbc.png")
    int_blocks_to_image(decrypted_cbc, size, mode, "decrypted_cbc.png")

if __name__ == "__main__":
    main()
