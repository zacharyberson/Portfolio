package dev.zberson.exception;

public abstract class CustomException {

    public static void printe(String e) {
        System.out.println("error: " + e);
    }

    public static final class InvalidUsernameException extends Exception {
        private static final long serialVersionUID = 20948L;

        public InvalidUsernameException() {
            super();
        }

        public InvalidUsernameException(String s) {
            super(s);
        }
    }
    
    public static final class InvalidReasonException extends Exception{
    	private static final long serialVersionUID = 21028102L;
    	
        public InvalidReasonException() {
        	super();
        }
        
        public InvalidReasonException(String s) {
        	super(s);
        }
    }
    
    public static final class InvalidFullNameException extends Exception{
    	private static final long serialVersionUID = 21012102L;
    	
        public InvalidFullNameException() {
        	super();
        }
        
        public InvalidFullNameException(String s) {
        	super(s);
        }
    }

    public static final class UsernameExistsException extends Exception {
        private static final long serialVersionUID = 1809234L;

        public UsernameExistsException() {
            super();
        }

        public UsernameExistsException(String s) {
            super(s);
        }
    }

    public static final class InvalidCommandException extends Exception {
        private static final long serialVersionUID = 6231L;

        public InvalidCommandException() {
            super();
        }

        public InvalidCommandException(String s) {
            super(s);
        }
    }

    public static final class InvalidCredentialsException extends Exception {
        private static final long serialVersionUID = 9876L;

        public InvalidCredentialsException() {
            super();
        }

        public InvalidCredentialsException(String s) {
            super(s);
        }
    }
    
    public static final class RequestAlreadyProcessedException extends Exception {
        private static final long serialVersionUID = 39039120L;

        public RequestAlreadyProcessedException() {
            super();
        }

        public RequestAlreadyProcessedException(String s) {
            super(s);
        }
    }
}