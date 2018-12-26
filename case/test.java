class MainClass{
	public static void main(String[] a){
        System.out.println(new test().max(1,2,3));
	}
}
class test{
    public int fun(int a,int b,int c){
        int m;
        if(a < b){
            m=b;
        }else{
            m=a;
        }
        if(m < c){
            m=c;
        }else{
            //
        }
        return m;
    }
}
class test2{
    public int one(){
        return 1;
    }
}
