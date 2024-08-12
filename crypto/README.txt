To run the TEA encryption program that encrypts and decrypts images.

After finish writing the code that includes all the necessary functions to do the job, you’ll run the program. The program will ask you to enter some information. You may be use the following:

- For the key, enter four parts in hexadecimal format:

  Enter key part 1 (hex): 12345678
  Enter key part 2 (hex): 2c3d4e5f
  Enter key part 3 (hex): 9b8a7f6e
  Enter key part 4 (hex): 0e0f1a1b
  
- For the initialization vector (IV), also in hexadecimal:
 
  Enter IV part 1 (hex): 1a2b3c4d
  Enter IV part 2 (hex): 9abcdef0

- For using the photo to test the encryption & decryption:
  Enter the path of the image to encrypt/decrypt: C:\Users\EASY LIFE\Desktop\Aqsa.png
(the image path in your device)


Once you provide all the required information, the program will encrypt and decrypt the image (Aqsa.png). It will then save the following files in the same folder as the project:

- 'encrypted_ecb.png' - The image after ECB encryption.
- 'decrypted_ecb.png' - The image after decrypting the ECB version.
- 'encrypted_cbc.png' - The image after CBC encryption.
- 'decrypted_cbc.png' - The image after decrypting the CBC version.

Then check the output pictures and sure that you have successfully run the encryption program and saved the images.
