/**
 * @author David Luo
 *         Represents an Appetizer item.
 */
public final class Appetizer extends Item {

    private static final double SALAD_PRICE = 4.5;
    private static final double SOUP_PRICE = 5.0;

    static final String APPETIZER_SELECTION_PROMPT =
            "Select an appetizer (1-2).\n" +
                    "1) Soup\n" +
                    "2) Salad";

    enum AppetizerType {
        SOUP("Soup"),
        SALAD("Salad");

        private String formattedName;

        AppetizerType(String formattedName) {
            this.formattedName = formattedName;
        }

        public String toString() {
            return formattedName;
        }
    }

    private AppetizerType appetizerType;

    /**
     * Constructor
     *
     * @param appetizerType The type of appetizer ordered.
     * @param quantity      How many appetizers of this type were ordered.
     */
    public Appetizer(AppetizerType appetizerType, int quantity) {
        super(quantity);

        // will complain if this is false.
        assert appetizerType != null;

        this.appetizerType = appetizerType;

        super.setName(appetizerType.toString());

        if (appetizerType == AppetizerType.SOUP)
            super.setPrice(SOUP_PRICE);
        else if (appetizerType == AppetizerType.SALAD) {
            super.setPrice(SALAD_PRICE);
        }
    }

    /**
     * @return What type of appetizer is in this order.
     */
    public AppetizerType getAppetizerType() {
        return appetizerType;
    }

    /**
     * Lists available appetizers.
     */
    public static String getMenu() {
        return "Appetizers\n" +
                Item.getMenuLine("Soup", SOUP_PRICE) +
                Item.getMenuLine("Salad", SALAD_PRICE);
    }

    /**
     * Mainly assists in the consolidation of duplicate items in an order.
     *
     * @param obj object to be compared.
     * @return If the objects have the same properties (i.e. same type of appetizer.)
     */
    @Override
    public boolean equals(Object obj) {
        return obj instanceof Appetizer &&
                ((Appetizer) obj).getAppetizerType().equals(getAppetizerType());
    }
}
