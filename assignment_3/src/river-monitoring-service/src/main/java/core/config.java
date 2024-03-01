package core;
public class config{

    public static final int F1 = 1000; //for normal state
    public static final int F2 = 500; //for all other states
    public static final int L0 = 0;  //valve opening level for ALARM-TOO-LOW state
    public static final int L1 = 25;  //valve opening level for normal state
    public static final int L2 = 50;  //valve opening level for ALARM-TOO-HIGH state
    public static final int L3 = 100; //valve opening level for ALARM-TOO-HIGH-CRITIC state

    public static final double WL1 = 1;  //lower water level for normal state
    public static final double WL2 = 2;  //upper water level for normal state
    public static final double WL3 = 3;  //PRE-ALARM-TOO-HIGH state water level
    public static final double WL4 = 4;  //ALARM-TOO-HIGH state water level

    public enum State{
        ALARM_TOO_LOW(L0,F2,"ALARM_TOO_LOW"),
        NORMAL(L1,F1,"NORMAL"),
        PRE_ALARM_TOO_HIGH(L1,F2,"PRE_ALARM_TOO_HIGH"),
        ALARM_TOO_HIGH(L2,F2,"ALARM_TOO_HIGH"),
        ALARM_TOO_HIGH_CRITIC(L3,F2,"ALARM_TOO_HIGH_CRITIC");

        public final int gateLevel;
        public final int updateFrequency;
        private final String name;

        State(int gateLevel, int updateFrequency, String name) {
            this.gateLevel = gateLevel;
            this.updateFrequency = updateFrequency;
            this.name = name;
        }

        @Override
        public String toString() {
            return this.name;
        }
    }
}