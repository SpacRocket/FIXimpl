# ba-thesis

## Build
The project is using Modern CMake for build system generation. Each part is developed as separate component and follows flat structure.
### Dependencies:
Quickfix (With OpenSSL dependency), OpenSSL, Google Test, date (HowardHinnant),
qRPC, cppcheck

## External processes
Bitfinex FIX Gateway (bfxfixgw): for more references visit medium tutorial

## List of ENV VARIABLES
FIX_SETTINGS_DIRECTORY - bfxfixgw configBFX_CLIENT_CONF - bfx client config
