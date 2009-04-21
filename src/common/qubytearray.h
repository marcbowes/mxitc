class QUByteRef {
  QUByteArray &a;
  int i;
  inline QUByteRef(QUByteArray &array, int idx)
    : a(array),i(idx) {}
  friend class QUByteArray;
public:
  inline operator const unsigned char() const
    { return i < a.d->size ? a.d->data[i] : 0; }
  inline QUByteRef &operator=(char c)
    { if (a.d->ref != 1 || i >= a.d->size) a.expand(i);
      a.d->data[i] = c;  return *this; }
  inline QUByteRef &operator=(const QUByteRef &c)
    { if (a.d->ref != 1 || i >= a.d->size) a.expand(i);
      a.d->data[i] = c.a.d->data[c.i];  return *this; }
  inline bool operator==(char c) const
    { return a.d->data[i] == c; }
  inline bool operator!=(char c) const
    { return a.d->data[i] != c; }
  inline bool operator>(char c) const
    { return a.d->data[i] > c; }
  inline bool operator>=(char c) const
    { return a.d->data[i] >= c; }
  inline bool operator<(char c) const
    { return a.d->data[i] < c; }
  inline bool operator<=(char c) const
    { return a.d->data[i] <= c; }
};

class QUByteArray : public QByteArray {
  inline QUByteRef QByteArray::operator[](int i)
    { Q_ASSERT(i >= 0); return QUByteRef(*this, i); }
  inline QUByteRef QByteArray::operator[](uint i)
    { return QUByteRef(*this, i); }
}

