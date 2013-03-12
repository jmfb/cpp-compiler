namespace A { namespace C {} }
namespace A { namespace D {} }
extern "C"
{
	extern "C" { namespace A { namespace B{} } }
}
static_assert(0, "something");

