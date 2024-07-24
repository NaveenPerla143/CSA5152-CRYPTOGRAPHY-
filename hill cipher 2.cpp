#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to find the modular inverse of a number
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Function to multiply two matrices and find the modulo
void matrixMultiply(int a[2][2], int b[2][1], int res[2][1], int mod) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 1; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
            res[i][j] %= mod;
        }
    }
}

// Function to encrypt the plaintext using Hill Cipher
void encrypt(char *plaintext, int key[2][2]) {
    int len = strlen(plaintext);
    if (len % 2 != 0) {
        // Padding with 'X' if length is odd
        strcat(plaintext, "X");
        len++;
    }
    
    for (int i = 0; i < len; i += 2) {
        int vector[2][1] = {{plaintext[i] - 'A'}, {plaintext[i + 1] - 'A'}};
        int result[2][1];
        
        matrixMultiply(key, vector, result, 26);
        
        plaintext[i] = (result[0][0] % 26) + 'A';
        plaintext[i + 1] = (result[1][0] % 26) + 'A';
    }
}

// Function to calculate determinant of 2x2 matrix
int determinant(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

// Function to find the inverse of the key matrix
void inverseKeyMatrix(int key[2][2], int invKey[2][2]) {
    int det = determinant(key);
    int detInv = modInverse(det, 26);
    
    invKey[0][0] = key[1][1] * detInv % 26;
    invKey[0][1] = -key[0][1] * detInv % 26;
    invKey[1][0] = -key[1][0] * detInv % 26;
    invKey[1][1] = key[0][0] * detInv % 26;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (invKey[i][j] < 0) {
                invKey[i][j] += 26;
            }
        }
    }
}

// Function to decrypt the ciphertext using Hill Cipher
void decrypt(char *ciphertext, int key[2][2]) {
    int invKey[2][2];
    inverseKeyMatrix(key, invKey);
    
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i += 2) {
        int vector[2][1] = {{ciphertext[i] - 'A'}, {ciphertext[i + 1] - 'A'}};
        int result[2][1];
        
        matrixMultiply(invKey, vector, result, 26);
        
        ciphertext[i] = (result[0][0] % 26) + 'A';
        ciphertext[i + 1] = (result[1][0] % 26) + 'A';
    }
}

int main() {
    char plaintext[100];
    char ciphertext[100];
    int key[2][2];

    // Input key matrix from the user
    printf("Enter the 2x2 key matrix:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            scanf("%d", &key[i][j]);
        }
    }

    // Display the entered key matrix
    printf("Key matrix entered:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d ", key[i][j]);
        }
        printf("\n");
    }
    
    // Input plaintext from the user
    printf("Enter plaintext: ");
    scanf("%s", plaintext);
    
    // Encrypt the plaintext
    strcpy(ciphertext, plaintext);
    encrypt(ciphertext, key);
    
    printf("Encrypted text: %s\n", ciphertext);
    
    // Decrypt the ciphertext
    decrypt(ciphertext, key);
    
    printf("Decrypted text: %s\n", ciphertext);
    
    return 0;
}
