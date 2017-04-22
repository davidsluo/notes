import java.io.Serializable;
import java.util.Date;

/**
 * A simple object representation of a Student
 *
 * @author David Luo
 */
public class Student extends Person implements Serializable {
    private String collegeName;

    /**
     * Constructor
     *
     * @param firstName   The student's first name
     * @param lastName    The student's last name
     * @param id          The student's unique ID
     * @param birthDate   The student's Date of Birth
     * @param collegeName The student's college
     */
    public Student(String firstName, String lastName, int id, Date birthDate, String collegeName) {
        super(firstName, lastName, id, birthDate);
        this.collegeName = collegeName;
    }

    /**
     * @return The student's college name
     */
    public String getCollegeName() {
        return collegeName;
    }

    /**
     * @param collegeName The student's college name
     */
    public void setCollegeName(String collegeName) {
        this.collegeName = collegeName;
    }

    /**
     * A string representation of the student
     */
    public String toString() {
        return super.toString() + " [" + getCollegeName() + "]";
    }

}
