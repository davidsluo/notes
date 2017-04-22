import java.io.Serializable;
import java.util.Date;

/**
 * A simple object representation of a Person
 *
 * @author David Luo
 */
public class Person implements Comparable<Person>, Serializable {

    private String firstName;
    private String lastName;
    private int id;
    private Date birthDate;

    /**
     * Constructor
     *
     * @param firstName The person's first name
     * @param lastName  The person's last name
     * @param id        The person's unique ID
     * @param birthDate The person's Date of Birth
     */
    public Person(String firstName, String lastName, int id, Date birthDate) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.birthDate = birthDate;

        if (id > 9999 || id < 0)
            throw new IllegalArgumentException("ID must be 4 digits.");

        this.id = id;
    }

    /**
     * @return The person's first name
     */
    public String getFirstName() {
        return firstName;
    }

    /**
     * @param firstName The person's first name
     */
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    /**
     * @return The person's last name
     */
    public String getLastName() {
        return lastName;
    }

    /**
     * @param lastName The person's last name
     */
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    /**
     * @return The person's unique ID
     */
    public int getId() {
        return id;
    }

    /**
     * @param id The person's unique ID
     */
    public void setId(int id) {
        this.id = id;
    }

    /**
     * @return The person's Date of Birth
     */
    public Date getBirthDate() {
        return birthDate;
    }

    /**
     * @param birthDate The person's Date of Birth
     */
    public void setBirthDate(Date birthDate) {
        this.birthDate = birthDate;
    }

    /**
     * @return A string representation of the person
     */
    @Override
    public String toString() {
        return String.format(
                "%1$04d\t%2$-10s\t%3$-10s\t%4$tm-%4$td-%4$tY",
                getId(), getFirstName(), getLastName(), getBirthDate());
    }

    /**
     * Compares this Person to another Person by ID
     * @param other The other person
     * @return 0 if equal;
     * 1 if this ID > other ID;
     * -1 if this ID < other ID
     */
    @Override
    public int compareTo(Person other) {
        return Integer.compare(this.getId(), other.getId());
    }

    /**
     * @param other The object to compare
     * @return If the object's ID's match. False if other is not comparable.
     */
    @Override
    public boolean equals(Object other) {
        try {
            return 0 == compareTo((Person) other);
        } catch (Exception e) {
            return false;
        }
    }

}
