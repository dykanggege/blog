pragma solidity ^0.4.19;


//合约
contract ZombieFactory {
    //uint无符号类型，其实是 uint256
    uint dnaDigits = 16;
    // ** 乘方
    uint dnaModulus = 10 ** dnaDigits;

    //结构体，自定义复合数据类型
    struct Zombie {
        string name;
        uint dna;
    }

    //数组，Zombie[3] 定长数组，Zombie[] 不定长数组，自动扩容
    //合约内声明的变量默认是 private，主动变为 public
    Zombie[] public zombies;


    //合约内声明的函数默认为 public，主动声明为 private
    //为了区别函数内部变量和全局变量，为内部变量命名前缀 _
    //私有函数也使用 _ 前缀
    function _createZombie(string _name, uint _dna) private {
        zombies.push(Zombie(_name, _dna));
    }

    //函数有很多种类型，如果它没有改变任何值或者写任何东西，可以看作 view 类型的函数
    // pure 函数, 表明这个函数甚至都不访问应用里的数据
    function _generateRandomDna(string _str) private view returns (uint) {
        // 这里开始
    }

}
