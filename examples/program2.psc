program abc;
var
a, b, c: integer;
d, e: integer;
begin
a = 5;
b = 10;
c = a + b * 2;

d = (c - a) / 3;
e = d + (b - a) * 2;

a = a + 1;
b = b - 2;
c = (a + b) * (d + 1);

show(a);
show(b);
show(c);
show(d);
show(e);
end

