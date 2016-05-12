using UnityEngine;

[behaviac.TypeMetaInfo()]
public class CustomPropertyAgent : behaviac.Agent
{
    public void resetProperties() {
        this.Variables.Clear(false);
    }

    public void init() {
        base.Init();
        resetProperties();
    }

    public void finl() {
    }

    public int _int_member = 0;

    [behaviac.MemberMetaInfo("", "")]
    public int IntProperty {
        get
        {
            return _int_member;
        }
        set
        {
            _int_member = value;
        }
    }

    public float _float_member = 0;

    [behaviac.MemberMetaInfo()]
    public float FloatPropertyReadonly {
        get
        {
            return _float_member;
        }
    }

    [behaviac.MemberMetaInfo()]
    public readonly bool BoolMemberReadonly = false;

    [behaviac.MemberMetaInfo()]
    public const int IntMemberConst = 0;

    [behaviac.MemberMetaInfo(true)]
    public string StringMemberReadonly = "read only sting";

    [behaviac.MemberMetaInfo()]
    public Vector3 Location = new Vector3();

    [behaviac.MethodMetaInfo()]
    public void FnWithOutParam(out int param) {
        param = 1;
    }
}
