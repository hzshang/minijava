class MainClass{
	public static void main(String[] a){
        System.out.println(new test().func(1,2));
	}
}
class test{
    int x;
    public int func(int a,int b){
        int c;
        int d;
        int t;
        if(c){
            t = c;
        }else{
            t = d;
            x = 1;
        }
        return a+b;
    }
}
