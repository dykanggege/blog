借助map实现了各个类型的set

- Difference是比较有意思的实现，用法如下
// Difference returns a set of objects that are not in s2
// For example:
// s1 = {a1, a2, a3}
// s2 = {a1, a2, a4, a5}
// s1.Difference(s2) = {a3}
// s2.Difference(s1) = {a4, a5}

- Union 并集
// s1 = {a1, a2}
// s2 = {a3, a4}
// s1.Union(s2) = {a1, a2, a3, a4}
// s2.Union(s1) = {a1, a2, a3, a4}

- Intersection 交集
// s1 = {a1, a2}
// s2 = {a2, a3}
// s1.Intersection(s2) = {a2}