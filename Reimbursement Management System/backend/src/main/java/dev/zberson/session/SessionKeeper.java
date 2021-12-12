package dev.zberson.session;

import java.util.HashMap;

public abstract class SessionKeeper {
    private static HashMap<Integer,ClientSession> sessions = new HashMap<Integer,ClientSession>();
    
    public static void addSession(int id, ClientSession s) {
        sessions.put(new Integer(id), s);
    }
    
    public static void removeSession(int id) {
        sessions.remove(new Integer(id));
    }
}
