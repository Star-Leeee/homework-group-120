#定义散列、ECDSA签名和ECDSA签名验证函数
from pycoin.ecdsa import generator_secp256k1, sign, verify
import hashlib, secrets

#哈希函数
def sha3_256Hash(msg):
    hashBytes = hashlib.sha3_256(msg.encode("utf8")).digest()
    return int.from_bytes(hashBytes, byteorder="big")
  
#该函数获取文本消息和 256 位 secp256k1 私钥，并计算 ECDSA 签名 {r， s} 并将其作为一对 256 位整数返回。
def signECDSAsecp256k1(msg, privKey):
    msgHash = sha3_256Hash(msg)
    signature = sign(generator_secp256k1, privKey, msgHash)
    return signature
  
#该函数获取文本消息、ECDSA 签名 {r， s} 和 2*256 位 ECDSA 公钥（未压缩），并返回签名是否有效。
def verifyECDSAsecp256k1(msg, signature, pubKey):
    msgHash = sha3_256Hash(msg)
    valid = verify(generator_secp256k1, pubKey, msgHash, signature)
    return valid


# ECDSA签署消息
msg = "Message for ECDSA signing"
privKey = secrets.randbelow(generator_secp256k1.order())
signature = signECDSAsecp256k1(msg, privKey)
print("消息:", msg)
print("私钥:", hex(privKey))
print("签名: r=" + hex(signature[0]) + ", s=" + hex(signature[1]))

# ECDSA验证签名
pubKey = (generator_secp256k1 * privKey).pair()
valid = verifyECDSAsecp256k1(msg, signature, pubKey)
print("\n消息:", msg)
print("私钥: (" + hex(pubKey[0]) + ", " + hex(pubKey[1]) + ")")
print("签名是否有效?", valid)

# ECDSA验证篡改签名
msg = "Tampered message"
valid = verifyECDSAsecp256k1(msg, signature, pubKey)
print("\n消息:", msg)
print("签名是否有效(篡改后)?", valid)
