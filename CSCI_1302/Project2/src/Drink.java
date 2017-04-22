/**
 * @author David Luo
 *         Represents a Drink.
 */
public final class Drink extends Item {

    private static final double DRINK_PRICE = 2.0;

    /**
     * Constructor
     *
     * @param quantity Number of drinks in this order.
     */
    public Drink(int quantity) {
        super(quantity);
        super.setName("Drink");
        super.setPrice(DRINK_PRICE);
    }

    /**
     * Prints the list of available drink(s) and their prices.
     */
    public static String getMenu() {
        return "Drinks\n" +
                Item.getMenuLine("Drink", DRINK_PRICE);
    }

    /**
     * Mainly assists in the consolidation of duplicate items in an order.
     *
     * @param obj the object to check for equality
     * @return if the two objects are both Drinks
     */
    public boolean equals(Object obj) {
        return obj instanceof Drink;
    }
}
