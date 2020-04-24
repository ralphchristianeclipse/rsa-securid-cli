// TokenAPISample.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <windows.h>
#include "stauto32.h"

// In order for this sample program to run correctly, at least
// one PINPad-style token should be imported first through the
// Desktop application

int main(int argc, char *argv[])
{
  // Define local vars
  LONG lTokenServiceHandle;
  LONG lTokens;
  LONG lDefaultToken;
  LONG lTimeLeft;
  DWORD dwBufferSize = 0;
  char chPIN[9];
  char chPRN[12];
  char chPASSCODE[12];
  // Open the Token Service and get a handle to it
  if (OpenTokenService(&lTokenServiceHandle) > 0)
  {
    // Get buffer size
    EnumToken(lTokenServiceHandle, &lTokens, &lDefaultToken, 0, &dwBufferSize);

    // Allocate memory
    LPTOKENBASICINFO lpTokens = new TOKENBASICINFO[lTokens];

    // List tokens
    if (EnumToken(lTokenServiceHandle, &lTokens, &lDefaultToken, lpTokens, &dwBufferSize) > 0)
    {
      // Select the first token, if there is one
      if (lTokens > 0)
      {
        // Get current codes with a PIN of "12345678"
        strcpy(chPIN, argv[1]);
        GetCurrentCode(lTokenServiceHandle, lpTokens[0].serialnumber, chPIN, &lTimeLeft, chPASSCODE, chPRN);
        printf("%s", chPASSCODE);
      }

      // Cleanup
      delete lpTokens;
    }

    // Close the Token Service
    CloseTokenService(lTokenServiceHandle);
  }

  return 0;
}
