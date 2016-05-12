[behaviac.TypeMetaInfo()]
public class FSMAgentTest : behaviac.Agent
{
    public enum EMessage {
        Invalid,
        Begin,
        End,
        Pause,
        Resume,
        Exit
    }

    public void resetProperties() {
        TestVar = -1;
        Message = EMessage.Invalid;
        this.Variables.Clear(false);
    }

    public void init() {
        base.Init();
        resetProperties();
    }

    public void finl() {
    }

    [behaviac.MemberMetaInfo()]
    public EMessage Message = EMessage.Invalid;

    public int TestVar = -1;

    [behaviac.MethodMetaInfo()]
    public void inactive_update() {
        TestVar++;
    }

    [behaviac.MethodMetaInfo()]
    public void active_update() {
        TestVar++;
    }

    [behaviac.MethodMetaInfo()]
    public void pause_update() {
        TestVar++;
    }

    [behaviac.MethodMetaInfo()]
    public void exit_update() {
        TestVar++;
    }
}
