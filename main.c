#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#define LINE_BUFFER_SIZE 64

void wmain()
{
  LPCWSTR fileName = L"./file2read.txt";
  HANDLE hFile = NULL;
  CHAR buffer[LINE_BUFFER_SIZE] = {0};
  CHAR lineBuffer[LINE_BUFFER_SIZE] = {0};
  DWORD dwBytesRead;
  UINT lineIndex = 0;

  // open file
  hFile = CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    printf("open file failed: %d\n", GetLastError());
    return;
  }

  while (ReadFile(hFile, buffer, LINE_BUFFER_SIZE, &dwBytesRead, NULL) && dwBytesRead > 0)
  {
    for (UINT i = 0; i < dwBytesRead; i++)
    {
      if (buffer[i] != '\n')
      {
        if (lineIndex < LINE_BUFFER_SIZE - 1)
        {
          lineBuffer[lineIndex++] = buffer[i];
        }
        else
        {
          printf("line too long, max size: %d\n", LINE_BUFFER_SIZE);
          // TODO: clean up
          return;
        }
      }
      else
      {
        if (lineIndex > 0 && lineBuffer[lineIndex - 1] == '\r')
        {
          lineBuffer[lineIndex - 1] = '\0';
        }
        lineBuffer[lineIndex] = '\0';
        lineIndex = 0;

        printf("read line: '%s'\n", lineBuffer);
      }
    }
  }

  if (lineIndex > 0)
  {
    if (lineBuffer[lineIndex - 1] == '\r')
    {
      lineBuffer[lineIndex - 1] = '\0';
    }
    lineBuffer[lineIndex] = '\0';
    printf("read line: '%s'\n", lineBuffer);
  }

  if (GetLastError() != 0)
  {
    printf("file read failed: %d\n", GetLastError());
  }
}
