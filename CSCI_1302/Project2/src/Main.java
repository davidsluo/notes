import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.List;
import java.util.Scanner;

/**
 * @author David Luo
 * @filename Main.java
 * @date October 6, 2016
 * @purpose Simulates a Pizza ordering program using Object Oriented Programming
 * (i.e. polymorphism, encapsulation, etc.)
 */
public class Main {

    private static List<Order> orders = new ArrayList<>();
    private static Scanner scanner = new Scanner(System.in);

    private final static String QUANTITY_PROMPT = "Enter Quantity";

    public static void main(String[] args) {

        boolean cont; // Stands for continue
        System.out.println("Welcome to <Pizza Place>.");

        // The main loop
        while (true) {
            // The catch everything block
            try {
                // Run until exit
                do {
                    cont = true;
                    Order order = new Order();
                    // Add more to order until exit
                    while (cont) {
                        Order.printMenu();
                        // Split into ordering appetizers, pizza, drinks
                        System.out.println(Order.OPTIONS_PROMPT);
                        switch (scanner.next()) {
                            // Order a pizza
                            case "1":
                                // Keep ordering while they want to order more
                                do {
                                    Pizza.Topping topping = null;
                                    Pizza.Size size = null;
                                    int pQuantity;

                                    // Map to array instead?
                                    // TODO: 10/3/2016 Make this prettier

                                    // Ask for size
                                    switch (getStrInput(Pizza.SIZE_SELECTION_PROMPT, "1", "2", "3")) {
                                        case "1":
                                            size = Pizza.Size.SMALL;
                                            break;
                                        case "2":
                                            size = Pizza.Size.MEDIUM;
                                            break;
                                        case "3":
                                            size = Pizza.Size.LARGE;
                                            break;
                                    }

                                    // Ask for topping
                                    switch (getStrInput(Pizza.TOPPING_SELECTION_PROMPT, "1", "2", "3")) {
                                        case "1":
                                            topping = Pizza.Topping.VEGETARIAN;
                                            break;
                                        case "2":
                                            topping = Pizza.Topping.CHEESE;
                                            break;
                                        case "3":
                                            topping = Pizza.Topping.PEPPERONI;
                                            break;
                                    }

                                    pQuantity = getQuantityInput(QUANTITY_PROMPT);

                                    order.addItem(new Pizza(topping, size, pQuantity));
                                    order.printInvoice();
                                } while (getYesNo("Order another pizza? (Y/N)"));
                                break;
                            // Order an Appetizer
                            case "2":
                                // Keep ordering while they want to order more
                                do {
                                    Appetizer.AppetizerType type = null;
                                    int aQuantity;

                                    // Ask for appetizer type
                                    switch (getStrInput(Appetizer.APPETIZER_SELECTION_PROMPT, "1", "2")) {
                                        case "1":
                                            type = Appetizer.AppetizerType.SOUP;
                                            break;
                                        case "2":
                                            type = Appetizer.AppetizerType.SALAD;
                                            break;
                                    }

                                    aQuantity = getQuantityInput(QUANTITY_PROMPT);

                                    order.addItem(new Appetizer(type, aQuantity));
                                    order.printInvoice();
                                } while (getYesNo("Order another appetizer? (Y/N)"));
                                break;
                            // Order Drinks
                            case "3":
                                // Keep ordering while they want to order more
                                do {
                                    // Ask for quantity of drinks (since there's only one choice)
                                    int dQuantity = getQuantityInput(QUANTITY_PROMPT);

                                    order.addItem(new Drink(dQuantity));
                                    order.printInvoice();
                                } while (getYesNo("Order another drink? (Y/N)"));
                                break;
                            default:
                                // Stop adding things to order
                                cont = false;
                                break;
                        }
                    }
                    order.printInvoice();
                    orders.add(order);

                } while (getStrInput("Order finished. Enter N for new customer or E to exit.", "n", "e").equals("n"));
                break;
            } catch (Exception | Error e) {
                // Should never happen, but just in case.
                if (getYesNo("Something went wrong. Exit? (Y/N)"))
                    break;
            }
        }

        printSummary();

    }

    /**
     * Helper method to print all orders made in this session.
     */
    private static void printSummary() {

        System.out.println("Invoices:\n");
        for (int i = 0; i < orders.size(); i++) {
            System.out.printf("Order %d\n", i + 1);
            orders.get(i).printInvoice();
        }

        System.out.println("Session Summary");
        System.out.println("Total number of orders:\t" + orders.size());
        System.out.println("Pizzas sold:           \t" + getTotalOrderCount(Pizza.class));
        System.out.println("Appetizers sold:       \t" + getTotalOrderCount(Appetizer.class));
        System.out.println("Drinks sold:           \t" + getTotalOrderCount(Drink.class));
    }

    /**
     * Abrevated version of {@link #getStrInput} for yes/no questions.
     *
     * @param query What to ask the user initially and whenever they enter an invalid input.
     * @return True/False respective to Yes/No
     */
    private static boolean getYesNo(String query) {
        return getStrInput(query, "y", "n").equals("y");
    }

    /**
     * Helper method to query user for a valid option from list of valid options.
     *
     * @param inputQuery  What to ask the user initially and whenever they enter an invalid input.
     * @param validInputs What can be considered valid input.
     * @return An element from validInputs
     */
    private static String getStrInput(String inputQuery, String... validInputs) {
        String input;

        // Keep asking for input until a valid one is returned.
        while (true) {
            System.out.println(inputQuery);

            input = scanner.next();

            for (String valid : validInputs) {
                if (valid.equalsIgnoreCase(input))
                    return input;
            }

            System.out.println("Invalid input.");
        }
    }

    /**
     * Helper method to query user for valid quantity.
     *
     * @param inputQuery What to ask the user initially and whenever they enter an invalid input.
     * @return A quantity between 1 and 100 inclusive.
     */
    private static int getQuantityInput(String inputQuery) {
        int input;

        // Keep asking for input until a valid one is returned.
        while (true) {
            System.out.println(inputQuery);

            try {
                // For some reason nextInt gave a weird infinite loop type situation.
                input = Integer.valueOf(scanner.next());
                if (input <= 0 || input > 100)
                    throw new NumberFormatException();
                else
                    return input;
            } catch (InputMismatchException | NumberFormatException e) {
//                System.out.println("Invalid input.");
                System.out.println("Input must be an integer between 1 and 100 (inclusive).");
            }
        }
    }

    /**
     * Helper class to get all of <code>type</code> items in all orders.
     *
     * @param type Type of item to count.
     * @return The number of items ordered in this session.
     */
    private static int getTotalOrderCount(Class type) {
        int totalCount = 0;
        for (Order order : orders) {
            totalCount += order.getCount(type);
        }

        return totalCount;
    }

}
