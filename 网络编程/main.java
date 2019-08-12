// 本题为考试单行多行输入输出规范示例，无需提交，不计分。
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        int n,k;
        String s;
        Scanner cin = new Scanner(System.in);
        n = cin.nextInt();
        k = cin.nextInt();
        s = cin.next();
        int[] num = new int[n+k];
        num[0] = s.charAt(0) == '1' ? 1:0;
        
        char[] cs = new char[n];

        cs[0] = s.charAt(0);
        for ( int i = 1; i < n; i++){
            if (!((s.charAt(i) == '1' ^ (num[i-1] % 2 == 1)))) {
                cs[i] = '0';
                num[i] = num[i-1];
            }else{
                cs[i] = '1';
                num[i] = num[i-1]+1;
            }
            if(i >= k-1){
                num[i] -= cs[i-(k-1)] == '1' ? 1 : 0;
            }
        }
        String ss = new String(cs);
        System.out.println(ss);
    }
}