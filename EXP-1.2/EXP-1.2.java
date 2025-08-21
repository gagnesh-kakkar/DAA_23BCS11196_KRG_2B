package DAA;

class PowerFunction {

    static double power(double x, int y) {
        if (y == 0) {
            return 1;
        }

        double temp = power(x, y / 2);

        if (y % 2 == 0) {
            return temp * temp;
        } else {
            if (y > 0) {
                return x * temp * temp;
            } else {
                return (temp * temp) / x;
            }
        }
    }

    public static void main(String[] args) {

        System.out.println("2^10 = " + power(2, 10));
        System.out.println("2^-3 = " + power(2, -3));
        System.out.println("5^0 = " + power(5, 0));
        System.out.println("3^7 = " + power(3, 7));
    }
}
