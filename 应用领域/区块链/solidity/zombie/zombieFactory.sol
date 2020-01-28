pragma solidity ^0.4.19;


//合约
contract ZombieFactory {
    //事件，用于给前端传递数据，像是封装过的 WebSocket
    //传入参数调用，就能通知到前端
    event NewZombie(uint zombieId, string name, uint dna);

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

    //映射关系，address 也是一种类型，用于存放用户地址
    mapping (uint => address) public  zombieToOwner;
    mapping (address => uint) ownerZombieCount;

    //合约内声明的函数默认为 public，主动声明为 private
    //为了区别函数内部变量和全局变量，为内部变量命名前缀 _
    //私有函数也使用 _ 前缀
    function _createZombie(string _name, uint _dna) internal {
        uint id = zombies.push(Zombie(_name, _dna)) - 1;
        zombieToOwner[id] = msg.sender;
        ownerZombieCount[msg.sender]++;
        NewZombie(id, _name, _dna);
    }

    //函数有很多种类型，如果它没有改变任何值或者写任何东西，可以看作 view 类型的函数
    // pure 函数, 表明这个函数甚至都不访问应用里的数据
    function _generateRandomDna(string _str) private view returns (uint) {
        uint rand = uint(keccak256(_str));
        return rand % dnaModulus;
    }

    //require 是一种强制要求，如果不满足要求则会报错并退出程序
    //solidity 中无法用 == 直接比较字符串，如果想比较字符串，需要用 hash 函数处理后比较
    //msg.sender 是内置常量，代表了使用该合同的当前用户的 address
    function createRandomZombie(string _name) public {
        require(ownerZombieCount[msg.sender] == 0);
        uint randDna = _generateRandomDna(_name);
        _createZombie(_name, randDna);
    } 

}
