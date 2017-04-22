/**
 * @author David Luo
 *         Represents a Pizza.
 */
public final class Pizza extends Item {

    private static final double S_VEGETARIAN_PRICE = 6.5;
    private static final double M_VEGETARIAN_PRICE = 10.5;
    private static final double L_VEGETARIAN_PRICE = 13.5;

    private static final double S_CHEESE_PRICE = 5.5;
    private static final double M_CHEESE_PRICE = 8.0;
    private static final double L_CHEESE_PRICE = 10.0;

    private static final double S_PEPPERONI_PRICE = 6.0;
    private static final double M_PEPPERONI_PRICE = 8.5;
    private static final double L_PEPPERONI_PRICE = 12.0;

    static final String TOPPING_SELECTION_PROMPT =
            "Select a topping (1-3).\n" +
                    "1) Vegetarian\n" +
                    "2) Cheese\n" +
                    "3) Pepperoni";

    static final String SIZE_SELECTION_PROMPT =
            "Select a size (1-3).\n" +
                    "1) Small\n" +
                    "2) Medium\n" +
                    "3) Large";

    enum Topping {
        VEGETARIAN("Vegetarian"),
        CHEESE("Cheese"),
        PEPPERONI("Pepperoni");

        private String formattedName;

        // Enums are essentially special inner classes and can have their own elements
        Topping(String formattedName) {
            this.formattedName = formattedName;
        }

        @Override
        public String toString() {
            return formattedName;
        }
    }

    enum Size {
        SMALL,
        MEDIUM,
        LARGE
    }

    // Should these be mutable or immutable?
    // i.e. to edit an order should I make a new Pizza object, or edit an existing one?
    // making new objects has slightly more overhead, but not that much
    // probably a matter of design or something.
    private Topping topping;
    private Size size;

    /**
     * Constructor
     *
     * @param topping  The topping on the pizza(s)
     * @param size     Size of pizza(s)
     * @param quantity Number of pizza(s)
     */
    public Pizza(Topping topping, Size size, int quantity) {
        super(quantity);
        // will complain if these is false.
        assert topping != null;
        assert size != null;

        this.topping = topping;
        this.size = size;

        // There's probably a way to make this prettier.
        if (topping.equals(Topping.VEGETARIAN) && size.equals(Size.SMALL)) {
            super.setPrice(S_VEGETARIAN_PRICE);
            super.setName("Small Vegetarian Pizza");
        } else if (topping.equals(Topping.VEGETARIAN) && size.equals(Size.MEDIUM)) {
            super.setPrice(M_VEGETARIAN_PRICE);
            super.setName("Medium Vegetarian Pizza");
        } else if (topping.equals(Topping.VEGETARIAN) && size.equals(Size.LARGE)) {
            super.setPrice(L_VEGETARIAN_PRICE);
            super.setName("Large Vegetarian Pizza");
        } else if (topping.equals(Topping.CHEESE) && size.equals(Size.SMALL)) {
            super.setPrice(S_CHEESE_PRICE);
            super.setName("Small Cheese Pizza");
        } else if (topping.equals(Topping.CHEESE) && size.equals(Size.MEDIUM)) {
            super.setPrice(M_CHEESE_PRICE);
            super.setName("Medium Cheese Pizza");
        } else if (topping.equals(Topping.CHEESE) && size.equals(Size.LARGE)) {
            super.setPrice(L_CHEESE_PRICE);
            super.setName("Large Cheese Pizza");
        } else if (topping.equals(Topping.PEPPERONI) && size.equals(Size.SMALL)) {
            super.setPrice(S_PEPPERONI_PRICE);
            super.setName("Small Pepperoni Pizza");
        } else if (topping.equals(Topping.PEPPERONI) && size.equals(Size.MEDIUM)) {
            super.setPrice(M_PEPPERONI_PRICE);
            super.setName("Medium Pepperoni Pizza");
        } else if (topping.equals(Topping.PEPPERONI) && size.equals(Size.LARGE)) {
            super.setPrice(L_PEPPERONI_PRICE);
            super.setName("Large Pepperoni Pizza");
        }
    }

    /**
     * @return Topping on the pizza(s)
     */
    public Topping getTopping() {
        return topping;
    }

    /**
     * @return Size of the pizza(s)
     */
    public Size getSize() {
        return size;
    }

    /**
     * Prints all topping/size combos and their respective prices in a fancy table.
     */
    public static String getMenu() {
        final String PIZZA_PATTERN = "%1$-12s $%2$-5.2f $%3$-5.2f $%4$-5.2f\n";

        return "Pizza\n" +
                String.format("%1$-12s %2$-6s %3$-6s %4$-6s\n",
                        "",
                        "Small",
                        "Medium",
                        "Large") +
                String.format(PIZZA_PATTERN,
                        "Vegetarian",
                        S_VEGETARIAN_PRICE,
                        M_VEGETARIAN_PRICE,
                        L_VEGETARIAN_PRICE) +
                String.format(PIZZA_PATTERN,
                        "Cheese",
                        S_CHEESE_PRICE,
                        M_CHEESE_PRICE,
                        L_CHEESE_PRICE) +
                String.format(PIZZA_PATTERN,
                        "Pepperoni",
                        S_PEPPERONI_PRICE,
                        M_PEPPERONI_PRICE,
                        L_PEPPERONI_PRICE);
    }

    /**
     * Mainly assists in the consolidation of duplicate items in an order.
     *
     * @param obj The object to be compared.
     * @return If the two Pizzas have the same toppings/sizes.
     */
    public boolean equals(Object obj) {
        return obj instanceof Pizza &&
                ((Pizza) obj).getSize().equals(getSize()) &&
                ((Pizza) obj).getTopping().equals(getTopping());
    }

}
