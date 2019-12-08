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

uf = []

def findRoot(a):
  if (uf[a] == a):
    return a;
    
  uf[a] = findRoot(uf[a])
  return uf[a]

def join(a, b):
  a = findRoot(a)
  b = findRoot(b)
  
  uf[a] = b

def main(argv):
  sys.setrecursionlimit(10**6)
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(3, 2000)
  vr.ReadSpace()
  
  K = vr.ReadInt(1, 2000)
  vr.ReadEndl();
  
  sumM = 0
  
  for i in range(N + 1):
    uf.append(i)
  A = [0] * (N + 1)
  
  for i in range(1, N + 1):
    A[i] = vr.ReadInt(1, N)
    vr.ReadSpace()
    
    assert A[i] != i, 'Found A_{} = {}'.format(i, i)
    
    join(i, A[i])
    
    M = vr.ReadInt(1, 10000)
    sumM = sumM + M
    
    before = -1
    
    for j in range(M):
      vr.ReadSpace()
      B = vr.ReadInt(before + 1, 10**9)
      before = B;
    
    vr.ReadEndl()
      
  assert sumM <= 10000, 'Sum of M exceeds 10000'
  
  for i in range(1, N + 1):
    assert A[A[i]] != i, 'Found A_{} = {} and A_{} = {}'.format(i, A[i], A[i], i)
  
  C = vr.ReadIntList(K, 0, 10**9)

  vr.ReadEof()


if __name__ == "__main__":
  sys.exit(main(sys.argv))
