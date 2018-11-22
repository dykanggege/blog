pragma solidity ^0.4.19;

//导入其他文件，没有命名空间？？？
import "./zombieFactory.sol";

//定义一个接口和定义合约，除了没做实现，别的没啥却别，
contract KittyInterface {
    //函数可以多返回值
    function getKitty(uint256 _id) external view returns (
        bool isGestating,
        bool isReady,
        uint256 cooldownIndex,
        uint256 nextActionAt,
        uint256 siringWithId,
        uint256 birthTime,
        uint256 matronId,
        uint256 sireId,
        uint256 generation,
        uint256 genes
    );
}

// is 继承，这个继承真奇葩
//合约内部修饰符：private 私有，public 共有，internal 内部和继承，external 外部，合约内部不能调用
contract ZombieFeeding is ZombieFactory {

    //发布的合约都会在以太坊链上存储着（分布式），如果我们想调用其他人的合约，必须先定义接口！
    //因为我们不知道他人的合约会不会改变，所以先定义接口，通过接口调用更安全
    //通过地址找到另一个合约，用接口做强制转换，这样才复合区块链的特征
    address ckAddress = 0x06012c8cf97BEaD5deAe237070F9587f8E7A266d;
    KittyInterface kittyContract = KittyInterface(ckAddress);


  function feedAndMultiply(uint _zombieId, uint _targetDna) public {
    require(msg.sender == zombieToOwner[_zombieId]);
    //合约中的全局变量，默认以 storage 方式存储，即，永久存在区块链中
    //函数中声明的变量，默认以 memory 方式存储，即，暂存在内存中，可以显示的改变存储方式
    Zombie storage myZombie = zombies[_zombieId];
    _targetDna = _targetDna % dnaModulus;
    uint newDna = (myZombie.dna + _targetDna) / 2;
    _createZombie("NoName", newDna);
  }

}
