package
{
	import flash.display.Sprite;

  import cmodule.cvtest.CLibInit;

	public class MyClass extends Sprite
	{	
    //C library loader
    private var lib:Object;

    //WebCam constructor and entry point
		public function MyClass():void
    {
      trace("Loading C++ library")
      var loader:CLibInit = new CLibInit();
      lib = loader.init();
      trace("Testing OpenCV interaction");
      trace(lib.test());
      trace("OK");
    }
  }
}
