#include "BfxApplication.hpp"

#include "quickfix/Session.h"

#ifdef HAVE_POSTGRESQL
#include "quickfix/PostgreSQLConnection.h"
#include "quickfix/PostgreSQLLog.h"
#include "quickfix/PostgreSQLStore.h"
#endif

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <string_view>

std::string CalcHmacSHA256(std::string_view decodedKey, std::string_view msg);
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len); 
std::string base64_decode(std::string const& encoded_string); 

namespace FIX {

BfxApplication::BfxApplication(){
  //Boilerplate
  gen = std::mt19937(time(nullptr));
  intDist = std::uniform_int_distribution<int>(0, INT_MAX);
}

void BfxApplication::onCreate(const SessionID &sessionID) {
  std::cout << YELLOW << "Session created - Session: " << RESET << sessionID.toString() << "\n";

  if(sessionID.getTargetCompID() == FIX::TargetCompID("BFXFIX") && 
     sessionID.getSenderCompID() == FIX::SenderCompID("EXORG_ORD"))
  {
  orderSessionID = sessionID;
  }
  else if(sessionID.getTargetCompID() == FIX::TargetCompID("BFXFIX") && 
     sessionID.getSenderCompID() == FIX::SenderCompID("EXORG_ORD"))
  {
  marketSessionID = sessionID;
  }

};

void BfxApplication::onLogon(const SessionID &sessionID) {
  std::cout << GREEN << "Logon - Session: " << RESET << sessionID.toString() << "\n";
}

void BfxApplication::onLogout(const SessionID &sessionID) {
  std::cout << RED << "Logout - Session: " << RESET << sessionID.toString() << "\n";
}

void BfxApplication::toAdmin(Message &message, const SessionID &sessionID) {

  FIX::MsgType msgType;
  message.getHeader().getField(msgType);

  if (msgType == MsgType_Logon) {
    const auto &defaultDict = settings.get();

    std::string secretDecrypted = base64_decode(defaultDict.getString("ApiSecret")); 
    std::string password = defaultDict.getString("Password");
    message.setField(FIX::Password(password));
  
    auto sendingTime{FIX::SendingTime()};
    message.setField(sendingTime);

    auto const& header = message.getHeader();
    auto msgSeqNum{header.getField(FIX::FIELD::MsgSeqNum)};
    
    std::ostringstream prehashStringStream;

    prehashStringStream << sendingTime.getString()
    << "\x01" << MsgType_Logon 
    << "\x01" << msgSeqNum 
    << "\x01" << sessionID.getSenderCompID().getString() 
    << "\x01" << sessionID.getTargetCompID().getString()
    << "\x01" << password;

    std::string prehashString{prehashStringStream.str()};

    std::string decryptedSignature{CalcHmacSHA256(secretDecrypted, prehashString)};
    
    FIX::RawData rawData{base64_encode(reinterpret_cast<unsigned char*>(const_cast<char*>(decryptedSignature.c_str()))
      ,decryptedSignature.length())};

    message.setField(rawData);

    FIX::CheckSum check{message.checkSum()};
    message.setField(check);
  }
}

void BfxApplication::toApp(Message &message, const SessionID &sessionID)
    EXCEPT(DoNotSend) {}

void BfxApplication::fromAdmin(const Message &message,
                               const SessionID &sessionID)
    EXCEPT(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon) {
}

void BfxApplication::fromApp(const Message &message, const SessionID &sessionID)
    EXCEPT(FieldNotFound, IncorrectDataFormat, IncorrectTagValue,
           UnsupportedMessageType) {
  crack(message, sessionID);
}

void BfxApplication::onMessage( const FIX44::ExecutionReport&, const FIX::SessionID& )
{ 
  //A structure of an order is being sent to the database
  
}

// Helper methods
FIX::TransactTime BfxApplication::getCurrentTransactTime() {
  using namespace std::chrono;
  using date::operator<<;

  auto tp = system_clock::now();
  std::stringstream utcTimeStamp;
  utcTimeStamp << tp;
  std::stringstream currentTime;
  FIX::TransactTime transactTime;
  transactTime.setString(currentTime.str());

  return transactTime;
}

FIX::ClOrdID BfxApplication::getCl0rdID(){
  return FIX::ClOrdID(std::to_string(intDist(gen)));
}

}  // namespace FIX

std::string CalcHmacSHA256(std::string_view decodedKey, std::string_view msg)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        decodedKey.data(),
        static_cast<int>(decodedKey.size()),
        reinterpret_cast<unsigned char const*>(msg.data()),
        static_cast<int>(msg.size()),
        hash.data(),
        &hashLen
    );

    return std::string{reinterpret_cast<char const*>(hash.data()), hashLen};
}

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}