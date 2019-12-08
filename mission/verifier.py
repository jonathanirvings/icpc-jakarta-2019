import re
import string
import sys


class VerifierReader(object):
  def __init__(self, text):
    self.text = text
    self.position = 0

  def HasNext(self):
    return self.position < len(self.text)

  def Read(self, target):
    actual = self.text[self.position : self.position + len(target)]
    assert actual == target, "Could not read '{0}'".format(target)
    self.position += len(target)

  def ReadSpace(self):
    self.Read(' ')

  def ReadEndl(self):
    self.Read('\n')

  def ReadEof(self):
    assert self.position == len(self.text), 'Found junk at the end of the file'

  def ReadChar(self):
    assert self.position < len(self.text), 'Unexpected EOF'
    char = self.text[self.position]
    self.position += 1
    return char

  def ReadLine(self, valid_characters):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      self.position += 1
      if c == '\n':
        break
      assert c in valid_characters
    assert self.position - old_position > 1, 'Nothing before next newline'
    return self.text[old_position:self.position - 1]

  def ReadRegex(self, target_regex):
    match = re.compile(target_regex).match(self.text, self.position)
    assert match is not None, 'Could not match /%s/' % target_regex
    self.position = match.end()
    return match.group(0)

  def ReadList(self, n, read_fn, *args):
    ls = []
    for i in range(n):
      if i > 0:
        self.Read(' ')
      ls.append(read_fn())
    self.ReadEndl()
    return ls

  def ReadInt(self, inclusive_min, inclusive_max):
    value = int(self.ReadRegex(r'0|(-?[1-9][0-9]*)'))
    assert inclusive_min <= value <= inclusive_max, (
        'Failed on %d <= %d <= %d' % (inclusive_min, value, inclusive_max))
    return value

  def ReadIntList(self, n, inclusive_min, inclusive_max):
    return self.ReadList(n, lambda: self.ReadInt(inclusive_min, inclusive_max))

  def ReadString(self, valid_characters=string.ascii_letters + string.digits,
                 inc_min_len=1, inc_max_len=10**6):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      if c not in valid_characters:
        break
      self.position += 1
    st = self.text[old_position:self.position]
    assert inc_min_len <= len(st) <= inc_max_len, (
        'Invalid string length: %d' % len(st))
    return st

  def ReadStringList(self, n,
                     valid_characters=string.ascii_letters + string.digits,
                     inc_min_len=1, inc_max_len=10**6):
    return self.ReadList(
        n,
        lambda: self.ReadString(valid_characters, inc_min_len, inc_max_len))


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(0, 50)
  vr.ReadSpace()
  xl, yl, xr, yr = vr.ReadIntList(4, 0, 10**3)

  assert xl < xr, 'Found xl >= xr.'
  assert yl < yr, 'Found yl >= yr.'

  xs = vr.ReadInt(xl + 1, xr - 1)
  vr.ReadSpace()
  ys = vr.ReadInt(yl + 1, yr - 1)
  vr.ReadEndl()

  xt = vr.ReadInt(xl + 1, xr - 1)
  vr.ReadSpace()
  yt = vr.ReadInt(yl + 1, yr - 1)
  vr.ReadEndl()

  assert (xs, ys) != (xt, yt), 'Found (xs, ys) = (xt, yt).'

  x = [0] * N
  y = [0] * N
  r = [0] * N

  for i in range(N):
    x[i] = vr.ReadInt(xl + 1, xr - 1)
    vr.ReadSpace()
    y[i] = vr.ReadInt(yl + 1, yr - 1)
    vr.ReadSpace()
    r[i] = vr.ReadInt(1, 10**3)
    vr.ReadEndl()

    assert xl + r[i] < x[i] < xr - r[i] and yl + r[i] < y[i] < yr - r[i], \
           'Sensor {} intersects grid.'.format(i)

  for i in range(N):
    for j in range(i + 1, N):
      assert (x[i] - x[j]) ** 2 + (y[i] - y[j]) ** 2 > (r[i] + r[j]) ** 2, \
             'Sensor {} intersects sensor {}.'.format(i, j)

  for i in range(N):
    assert (x[i] - xs) ** 2 + (y[i] - ys) ** 2 > r[i] ** 2, \
           'Sensor {} intersects (xs, ys).'.format(i)

    assert (x[i] - xt) ** 2 + (y[i] - yt) ** 2 > r[i] ** 2, \
           'Sensor {} intersects (xt, yt).'.format(i)

  vr.ReadEof()


if __name__ == "__main__":
  sys.exit(main(sys.argv))
