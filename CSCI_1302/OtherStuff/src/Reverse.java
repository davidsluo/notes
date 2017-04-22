import java.util.Stack;

/**
 * Reverses a string using a stack.
 *
 * @author David Luo
 */

public class Reverse {

    private static Stack<Character> stack = new Stack<Character>();

    public static String reverse(String s) {
        for (char c : s.toCharArray()) {
            stack.push(c);
        }

        String output = "";

        while (!stack.isEmpty()) {
            output += stack.pop();
        }

        return output;
    }

    public static void main(String[] args) {
        // Prints "otatop"
        System.out.println(Reverse.reverse("potato"));
        // Prints "54321"
        System.out.println(Reverse.reverse("12345"));
        // Prints "zyxwvutsrqponmlkjihgfedcba"
        System.out.println(Reverse.reverse("abcdefghijklmnopqrstuvwxyz"));
    }
}