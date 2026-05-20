package com.example.project_1200308_1201738;
import android.util.Base64;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

public class AESUtil {

    private static final String AES = "AES";

    public static String generateKey() throws Exception {
        KeyGenerator keyGen = KeyGenerator.getInstance(AES);
        keyGen.init(128);
        SecretKey secretKey = keyGen.generateKey();
        return Base64.encodeToString(secretKey.getEncoded(), Base64.DEFAULT);
    }

    public static String encrypt(String key, String data) throws Exception {
        SecretKeySpec secretKeySpec = new SecretKeySpec(Base64.decode(key, Base64.DEFAULT), AES);
        Cipher cipher = Cipher.getInstance(AES);
        cipher.init(Cipher.ENCRYPT_MODE, secretKeySpec);
        byte[] encrypted = cipher.doFinal(data.getBytes());
        return Base64.encodeToString(encrypted, Base64.DEFAULT);
    }

    public static String decrypt(String key, String encryptedData) throws Exception {
        SecretKeySpec secretKeySpec = new SecretKeySpec(Base64.decode(key, Base64.DEFAULT), AES);
        Cipher cipher = Cipher.getInstance(AES);
        cipher.init(Cipher.DECRYPT_MODE, secretKeySpec);
        byte[] decrypted = cipher.doFinal(Base64.decode(encryptedData, Base64.DEFAULT));
        return new String(decrypted);
    }
}
