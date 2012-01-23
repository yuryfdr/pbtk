/* Copyright (C) 2009 AI */
/* Copyright (C) 2011 Yury P. Fedorchenko (yuryfdr at users.sf.net)  */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
#include "convert.h"

/*  UTF-8
 *
 *  Bits  Pattern
 *  ----  -------
 *    7   0xxxxxxx
 *   11   110xxxxx 10xxxxxx
 *   16   1110xxxx 10xxxxxx 10xxxxxx
 *   21   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *   26   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *   32   111111xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 */

enum {
  Low6Bits = 0x3F,              /* 00111111 */
  High2Bits = 0xC0,             /* 11000000 */
  ByteMask = 0x00BF,            /* 10111111 */
  ContinueBits = 0x80           /* 10xxxxxx */
};

void to_utf8(const char *from, char *to, const unsigned short *encoding)
{
  unsigned int i, j;
  for (i = 0, j = 0; i < strlen(from); i++) {
    unsigned short ch = (unsigned char) from[i];

    if (ch >= 0200) {
      ch = encoding[ch - 0200];
    }

    if (ch < ContinueBits) {
      to[j++] = ch;
    } else if (ch < 0x800) {
      to[j++] = (ch >> 6 | High2Bits);
      to[j++] = ((ch & Low6Bits) | ContinueBits);
    } else {
      to[j++] = (ch >> 12 | 0xe0);
      to[j++] = (((ch >> 6) & Low6Bits) | ContinueBits);
      to[j++] = ((ch & Low6Bits) | ContinueBits);
    }

  }

  to[j] = '\0';
}

void to_utf8(const char *from, std::string * to, const unsigned short *encoding)
{
  if (from == 0) {
    to->clear();
    return;
  }

  char *buf = new char[strlen((const char *) from) * 3 + 1];
  to_utf8(from, buf, encoding);

  to->assign(buf);
  delete[]buf;
}

std::string to_utf8(const char *from, const unsigned short *encoding)
{
  std::string to;
  if (from == 0) {
    to.clear();
    return to;
  }

  char *buf = new char[strlen(from) * 3 + 1];
  to_utf8(from, buf, encoding);

  to.assign(buf);
  delete[]buf;
  return to;
}

std::string utf8_to(const char *from, const unsigned short *encoding)
{
  std::string to;
  if (from == 0)
    return to;

  int len = strlen(from);

  for (int i = 0; i < len; i++) {
    char ch(0x20);
    if ((unsigned char) from[i] < ContinueBits)
      ch = (unsigned char) from[i];
    else if (i < len - 1) {
      wchar_t wch;
      wch = (((unsigned char) from[i] & Low6Bits) << 6) | ((unsigned char) from[i + 1] & Low6Bits);
      for (int c = 127; c >= 0; --c)
        if (encoding[c] == wch) {
          ch = (char) (c + ContinueBits);
          i++;
          break;
        }
    }

    to.append(1, ch);
  }

  return to;
}
