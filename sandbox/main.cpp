#include <RheelEngine.h>

using namespace rheel;

class FpsUpdater : public ComponentBase {

public:
	void SetElement(TextElement* element) {
		_element = element;
	}

	void Update() override {
		if (_element) {
			_element->text = std::to_string(int(std::round(1.0 / GetTimeDelta()))) + " FPS";
		}
	}

private:
	TextElement* _element = nullptr;

};

class LoremElement : public Element {

protected:
	virtual void DoDraw(float time, float dt) const override {
		GetTextRenderer().DrawParagraph(
				"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc nec maximus lectus. Vivamus eget neque eros. Etiam sit amet hendrerit elit, quis accumsan lorem. Proin at turpis sed neque faucibus ullamcorper vitae ut arcu. Nunc nulla nibh, congue sed ipsum volutpat, aliquet fringilla tellus. Etiam at libero vel leo faucibus sagittis at ac lacus. Vestibulum a volutpat eros. Vestibulum sed lorem erat.\n"
				"In volutpat tellus quis ante ultrices semper. Sed eu hendrerit tellus. Vestibulum consequat tincidunt elementum. Curabitur efficitur odio lacinia mi sodales iaculis. Ut dictum leo ut tellus facilisis, vel porta elit varius. Proin maximus magna quis libero euismod, nec malesuada quam mollis. Quisque posuere congue dolor vitae luctus. Pellentesque ornare diam vel vestibulum mollis. Etiam bibendum lacinia ex, ac fermentum nulla vehicula a. Vivamus posuere in dolor non bibendum. Mauris a ultrices tellus. Vivamus nibh velit, lacinia facilisis enim vitae, semper posuere est. Morbi porta metus et laoreet facilisis. Curabitur et tellus et metus molestie fringilla auctor ac lorem.\n"
				"Proin interdum quam nulla, vitae dignissim lacus sodales ut. Proin posuere iaculis fermentum. Vivamus sed tortor egestas, consectetur elit vitae, faucibus arcu. Morbi condimentum felis sit amet sodales congue. Vivamus at tempor odio. Maecenas in sapien fringilla, rutrum ipsum non, vulputate neque. In hac habitasse platea dictumst. Curabitur eget ex sit amet sem luctus laoreet. Vivamus interdum turpis est. In ac pretium dui. Donec fringilla lacus vel libero facilisis, eget vehicula lorem tincidunt. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\n"
				"Duis pretium lacus eget est vestibulum hendrerit. Pellentesque ultrices pretium facilisis. Phasellus volutpat nunc iaculis neque posuere posuere. Duis non elit risus. Aliquam volutpat quam a lectus blandit scelerisque. Vestibulum ut lectus lacus. Ut a sapien fermentum, suscipit odio a, vulputate turpis. Fusce ante turpis, consequat a lacinia id, mattis et arcu. Nunc tincidunt ante eget enim posuere efficitur.\n"
				"Morbi commodo ligula a justo pharetra suscipit. Maecenas non nisi eu diam lacinia ullamcorper id in justo. Ut ex nisi, tincidunt pulvinar velit quis, congue placerat enim. Suspendisse vitae varius lacus. Sed vel euismod nisl. Sed ornare ac lacus non consequat. Curabitur quis urna nulla. Sed suscipit volutpat commodo. Nam leo sapien, placerat nec gravida ut, suscipit et nunc. Ut iaculis risus quam, sit amet tincidunt neque fringilla vitae. Curabitur sit amet dolor suscipit, porta ligula vel, fermentum mi. Mauris ornare ligula ac purus pretium malesuada. Nullam in tempor eros. Proin nec felis sit amet lorem dignissim tristique in et odio.\n"
				"Praesent vestibulum est nulla, in pretium dui blandit non. Nullam consectetur commodo leo, a pretium nunc dignissim sed. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Curabitur eros mauris, fringilla sit amet turpis aliquam, pharetra tincidunt est. Curabitur nec erat laoreet, porta mi sed, ultrices sem. Nunc lectus urna, imperdiet ac fermentum pellentesque, dapibus vitae velit. Nunc euismod, nunc et pulvinar iaculis, ex lacus lacinia eros, sit amet tincidunt elit ligula nec ligula. Ut convallis, enim sit amet viverra imperdiet, elit orci consequat est, sed auctor lacus dui sed erat. Duis laoreet fringilla nulla, non pretium lacus sollicitudin non. Donec pretium bibendum accumsan. Sed tincidunt aliquet sapien a aliquam. Aliquam scelerisque purus lacinia nisl molestie tincidunt. In tempor vulputate turpis. Nulla consequat nibh nulla, nec semper lacus blandit quis. Suspendisse potenti.\n"
				"Vestibulum mattis ante lacinia, elementum purus et, pharetra lacus. Nulla urna quam, aliquam hendrerit egestas faucibus, malesuada a ante. Quisque diam mauris, fermentum vel ante in, rutrum laoreet magna. Sed eget odio sodales, sollicitudin ante id, dictum sapien. Aenean vulputate eu leo sed dapibus. Quisque a mi vel orci consectetur fringilla. Etiam volutpat quis erat vel eleifend. Pellentesque tincidunt magna a libero vulputate pretium. Etiam aliquam magna consectetur lacus volutpat, eget commodo diam ultricies. Maecenas volutpat placerat molestie. Phasellus blandit, purus sit amet pellentesque pellentesque, lacus elit iaculis sapien, ullamcorper tristique mauris lectus eu felis. Nulla id orci felis. Proin sit amet lorem quis lectus lobortis suscipit.\n"
				"Quisque feugiat et nulla at tincidunt. Duis dictum et metus non vestibulum. Sed convallis efficitur dolor, eget laoreet magna vehicula at. Donec scelerisque dapibus quam, ut faucibus nibh imperdiet ut. Praesent id feugiat dolor. In venenatis lorem ut commodo egestas. Duis maximus vulputate nunc, in placerat sapien iaculis vitae. Proin non iaculis libero.\n"
				"Sed nec scelerisque libero. Proin venenatis elit at ipsum volutpat vulputate quis eget sem. Donec a mauris odio. Donec interdum nibh eget leo blandit, ac auctor sem sagittis. Phasellus mollis, ipsum nec venenatis convallis, ante libero egestas risus, sit amet feugiat orci purus sit amet risus. Aliquam malesuada metus facilisis orci faucibus ultricies. Nullam interdum lectus dui, et vehicula dolor ultricies ut. Ut nisi tellus, efficitur sed nisl ultrices, rhoncus rutrum enim.\n"
				"Ut porta ligula mauris, quis vehicula risus blandit eu. Aenean porta sit amet nulla in aliquet. Nam eu libero sollicitudin, pellentesque augue id, euismod elit. Nam eu nisi rutrum, placerat nisl sed, eleifend urna. Sed hendrerit augue quis neque iaculis, nec sollicitudin neque congue. Aenean non sapien diam. Vestibulum interdum, neque non egestas sodales, neque orci laoreet dui, in laoreet eros arcu nec diam. Praesent ut tortor vel orci suscipit fringilla in quis mauris. Nunc placerat lectus quis pellentesque blandit. Curabitur quis nibh orci. Duis sagittis vehicula lectus, vel interdum leo sollicitudin nec. Vivamus id lorem eu nunc faucibus sodales.\n"
				"Nullam finibus euismod urna, eget finibus justo placerat non. Vestibulum eleifend tincidunt scelerisque. In in nisl sem. Curabitur malesuada odio at commodo porta. Ut malesuada libero quam, molestie luctus tellus semper vel. Curabitur eu augue nulla. Ut vehicula sed quam eu faucibus. Nulla ultricies egestas massa a feugiat. Morbi ex lorem, rhoncus at tellus vel, placerat pellentesque tellus. Aenean consequat dignissim nunc eget vehicula. Morbi tellus massa, imperdiet a pulvinar id, viverra blandit lacus. Nunc mattis suscipit libero ac rutrum. Quisque ut rutrum nisl, ut ultrices ante. Quisque id lorem et diam blandit ornare. Proin a metus ornare, cursus odio nec, placerat est. Ut lectus sapien, dapibus eu ultrices non, blandit vitae augue.\n"
				"Mauris luctus diam eget odio tempor volutpat. Aenean aliquet sagittis velit. Sed nec nisi sit amet risus interdum iaculis et et nisl. Pellentesque a eleifend ipsum. Proin placerat justo sit amet arcu malesuada consequat. Aliquam eu consectetur risus. Nullam eleifend posuere orci, nec ullamcorper massa aliquam quis.\n"
				"Pellentesque facilisis tellus congue, pharetra metus in, condimentum elit. Vivamus ut eros quis tortor commodo laoreet sed ut velit. Duis ex lectus, fermentum at odio eu, sagittis scelerisque felis. Mauris vulputate eros eu purus convallis condimentum. Curabitur vehicula turpis et tellus maximus facilisis. Integer congue consequat vestibulum. Sed et iaculis elit. Duis ultrices dolor ac erat scelerisque gravida. Proin id ultrices lorem, sodales maximus erat. Phasellus viverra efficitur metus laoreet efficitur. Nam rutrum nisi vitae varius scelerisque. Sed porttitor id nulla at porta. Donec eu arcu in justo consequat gravida vitae a massa.\n"
				"Donec et finibus ipsum, ac venenatis dolor. Suspendisse venenatis quam felis, id auctor dui tincidunt eu. Nunc luctus sodales tortor at mollis. Morbi vel dignissim tortor, ut mollis sapien. Integer eu purus pharetra, varius eros eget, convallis nulla. Praesent et mauris a tellus facilisis fringilla. Mauris gravida orci at faucibus tristique. Sed posuere arcu condimentum, eleifend arcu eget, auctor lectus. Etiam elementum venenatis lectus ut laoreet. Praesent consequat elit ornare sapien euismod, sit amet convallis justo congue.\n"
				"Vivamus non erat nec urna ullamcorper hendrerit sed nec risus. Praesent posuere, libero a gravida blandit, magna quam facilisis arcu, non tincidunt odio ante sit amet purus. Ut ornare mauris at ipsum lacinia, non tempor turpis consequat. Suspendisse sollicitudin, est a eleifend interdum, ipsum enim porttitor massa, vel imperdiet urna tortor vel ipsum. Pellentesque lacinia tincidunt purus a hendrerit. Suspendisse felis libero, condimentum eu posuere nec, elementum non elit. Morbi massa mauris, placerat in lobortis eu, pretium a ligula. Proin et sapien a tortor viverra sodales.\n"
				"In hac habitasse platea dictumst. Nunc congue ac ipsum in feugiat. Aliquam consectetur, risus et volutpat vehicula, nibh est mollis mi, sed mattis quam neque non eros. Vivamus pellentesque velit id cursus ornare. Vestibulum dapibus pretium ante sit amet rhoncus. Aenean malesuada nunc gravida sapien iaculis convallis id nec dolor. Nunc vestibulum rutrum nulla, non faucibus sem dignissim ut. Donec ac tortor turpis. Quisque porta, lacus sit amet gravida lobortis, lorem ipsum consectetur turpis, id maximus quam lectus eget mi. Etiam ac orci ut diam dictum efficitur. Aenean accumsan, ipsum sit amet mattis lobortis, magna turpis rhoncus nulla, eget suscipit leo ante et eros. Ut bibendum magna in justo mollis, nec malesuada erat dictum. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\n"
				"In ipsum massa, suscipit a turpis id, imperdiet tincidunt nisl. Aenean lobortis dui eget tellus luctus, sit amet egestas mi consectetur. Proin eu nulla vel velit pharetra varius eget non tortor. Integer venenatis neque non ligula efficitur, in vestibulum quam egestas. Suspendisse in massa eros. Donec ex lacus, blandit non felis non, posuere mollis elit. Duis fringilla id est bibendum maximus. Donec ultricies consequat massa, et venenatis enim pharetra id. Suspendisse vehicula risus sit amet lectus sollicitudin aliquam. Ut tristique eleifend lorem, et elementum urna eleifend ut. Nunc elementum ligula at mauris lacinia, et sodales enim egestas. Nam vehicula venenatis leo id gravida.\n"
				"Vestibulum elementum rhoncus erat nec aliquam. Fusce facilisis tincidunt mi, vitae feugiat nisi convallis eu. Praesent placerat in felis at luctus. Quisque sagittis aliquet tempor. Maecenas ut augue ac augue bibendum mattis. Donec laoreet feugiat ligula ac sollicitudin. Sed placerat eros eu urna malesuada euismod. Vivamus consequat viverra ipsum eu tempor. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Sed a scelerisque massa, non posuere quam. Fusce non porttitor magna, et sodales felis.\n"
				"Aenean at aliquet lectus. Vestibulum nulla massa, blandit efficitur imperdiet vestibulum, accumsan nec urna. Proin ornare finibus placerat. Donec efficitur nisl nec tellus molestie malesuada. Fusce vel libero tortor. Nullam feugiat elementum odio, at semper mauris facilisis nec. Pellentesque quis velit non tortor mollis aliquam at et quam. Suspendisse vel bibendum velit. Phasellus ut rutrum justo. Integer malesuada vulputate dui, sit amet tincidunt leo condimentum a. Aenean non erat id lectus facilisis elementum. Morbi et augue in ex ultrices hendrerit eget a massa. Praesent semper lobortis libero sed posuere.\n"
				"Vestibulum quis egestas arcu. Sed at mattis purus. Proin libero libero, facilisis ultricies ultrices eu, pharetra vel sem. Nunc vitae leo faucibus, volutpat justo sed, scelerisque nisi. Vivamus sed imperdiet nulla. Ut enim odio, cursus sit amet nulla quis, sollicitudin eleifend sapien. Donec gravida odio sed dapibus euismod.\n"
				"Nunc nec tempor erat. Donec suscipit mauris orci, et rutrum sapien varius vel. Aliquam ac eleifend felis, vel vulputate lectus. Vestibulum sem mauris, ultricies eu urna vitae, faucibus pulvinar turpis. In eleifend auctor ligula. Sed hendrerit, odio a efficitur aliquet, diam enim auctor magna, at auctor nisi dui et enim. Integer condimentum sapien sed justo vulputate, et ullamcorper nisi mattis. Pellentesque arcu nisi, maximus eget sem in, scelerisque pellentesque libero. Maecenas vehicula eu dolor non dignissim. Sed sem ex, aliquam ut luctus in, rutrum a lacus. Proin libero massa, egestas eget maximus non, laoreet ac mi. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Aliquam ac aliquet nulla. Nam id tempor odio. Aenean purus urna, fermentum vitae accumsan a, viverra ac tellus.\n"
				"Maecenas nec dolor quis augue iaculis aliquet. Phasellus ac sollicitudin nunc, et consequat nisl. Aenean ut molestie sapien. Ut hendrerit ipsum eget justo dapibus euismod. Suspendisse gravida non est sit amet congue. Praesent ullamcorper nunc nisi, ac imperdiet quam dignissim id. Nunc condimentum elementum diam quis condimentum. Cras quis lectus eget odio pulvinar consectetur quis quis nisl. Pellentesque sit amet est scelerisque, malesuada erat et, pretium odio. Mauris odio mi, mollis in sapien non, elementum aliquam libero. Nullam sollicitudin elit et pretium feugiat. Quisque purus dui, rhoncus id mi ac, ultrices interdum tortor. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Aliquam ornare blandit leo, ac lobortis quam. Morbi interdum urna nec pulvinar cursus.\n"
				"Praesent elementum gravida est a condimentum. Morbi nisl enim, interdum nec tellus et, fringilla pulvinar metus. Aenean eu urna tortor. Nullam est nisi, ullamcorper a rhoncus ullamcorper, ullamcorper id neque. Nunc in elit at ipsum auctor porttitor. Maecenas consectetur congue tortor a sagittis. Suspendisse justo risus, luctus sit amet eleifend vel, eleifend ac leo. Maecenas porttitor neque mauris, vitae tempor ex finibus quis. Integer ac sapien eget sem tincidunt sagittis ut vitae elit. Vestibulum sed fermentum risus, non tristique ipsum. Curabitur neque lectus, scelerisque id est nec, feugiat rutrum massa.\n"
				"Curabitur suscipit ante non nulla rhoncus, eu vulputate velit porta. Maecenas vehicula, arcu eu varius elementum, dolor tortor lacinia leo, id fringilla nibh velit eu orci. In nibh erat, sagittis vel venenatis vitae, pellentesque eu sem. Sed mattis non lectus vel rutrum. Duis egestas aliquam sapien vel tempor. Quisque sed pretium orci. Vestibulum id pulvinar leo. Donec pretium ligula massa, a efficitur lacus lobortis in. Duis vehicula massa est, et porttitor felis accumsan id. Nulla facilisi. Suspendisse pellentesque tellus est, nec egestas nulla lacinia sit amet. Vestibulum sagittis imperdiet orci a vulputate. Quisque lobortis eu ligula eu congue. Maecenas ornare nunc ultricies lacus blandit, vel iaculis enim cursus.\n"
				"Cras elit ligula, lacinia id feugiat sit amet, faucibus eu ante. Donec et quam justo. Sed a pretium enim, id ornare libero. Mauris ut commodo dui, eu ullamcorper sapien. Etiam ipsum eros, eleifend a neque quis, condimentum semper nisl. Proin turpis lectus, aliquam ac auctor sit amet, suscipit ut mauris. Nunc porta velit vel nisl dapibus viverra. Mauris in scelerisque arcu, rutrum iaculis mi. Nulla feugiat venenatis nisi, nec iaculis enim laoreet sed. Quisque quis neque id tortor tincidunt dapibus quis eget ligula. Nunc et lacus tristique dui imperdiet consectetur. Nulla ullamcorper mi vitae gravida finibus. In finibus risus consectetur dapibus tristique. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed euismod ipsum mauris, a pulvinar enim vulputate a.\n"
				"Nunc eu turpis turpis. Proin gravida in est eu luctus. Aenean sit amet augue vel libero pellentesque ultrices quis vel ipsum. Pellentesque at congue lacus. Integer sagittis est in ligula ultricies pulvinar. Nam ut nulla laoreet, laoreet dui non, blandit justo. Fusce id nulla et quam suscipit blandit imperdiet at neque. Vestibulum a nisi gravida, elementum sapien in, suscipit odio. Ut turpis est, faucibus in pellentesque vel, eleifend et lorem. Proin pretium hendrerit dui eu congue. Mauris commodo porta lorem, sed fermentum ante auctor congue.\n"
				"Vivamus nec ultricies magna, id ullamcorper felis. Phasellus sapien leo, lacinia vel dignissim malesuada, suscipit non nibh. Morbi euismod suscipit accumsan. Cras euismod varius leo, non tempus lorem fringilla in. Praesent sagittis ut dolor vel eleifend. Vivamus id tempus arcu. Duis interdum, tortor ac ultrices elementum, ipsum tellus feugiat turpis, ut tempor nulla dolor vitae nisi. Pellentesque faucibus eros vitae rhoncus accumsan. Nulla tempor hendrerit augue, sed faucibus ligula sodales vitae.\n"
				"Nam convallis leo eros, sodales blandit nibh elementum vel. Ut vulputate feugiat consectetur. Sed odio elit, lacinia at libero eget, sodales sollicitudin sapien. Nulla sit amet turpis efficitur, porta mauris eget, luctus leo. Vestibulum sit amet nibh vulputate, mollis risus vel, finibus turpis. Ut quam mauris, consequat id pellentesque quis, fermentum id purus. Nam viverra ante nunc, sed dapibus dui placerat ut. Aenean aliquam auctor sem, eu varius mi euismod a. Nunc elementum ultrices mi. Fusce ac orci a est maximus luctus vitae id nibh. Donec vitae viverra tellus, id eleifend leo. Vestibulum sed commodo eros, nec varius ipsum. Fusce lectus ipsum, mollis in orci nec, gravida placerat libero. Vestibulum in malesuada ligula. Nulla vehicula a ipsum sed tristique. Nunc blandit pulvinar libero.\n"
				"Interdum et malesuada fames ac ante ipsum primis in faucibus. Cras porta tempor ipsum, sed convallis felis euismod sed. Duis tempus dui quis velit volutpat consectetur. Nunc diam quam, venenatis sit amet commodo ut, faucibus vitae massa. Suspendisse nisl risus, rhoncus non eros eu, pharetra mollis erat. In non nunc vitae dolor efficitur dignissim faucibus nec quam. Vestibulum nibh libero, pharetra at ipsum a, dictum accumsan felis. Nam tristique congue libero, quis cursus nisi sodales sit amet. Sed interdum vestibulum odio, consequat aliquam orci sollicitudin at. Nullam eleifend faucibus sodales.\n"
				"Cras eget purus quis turpis dapibus condimentum. Curabitur tellus quam, maximus non eros at, rhoncus aliquet nisi. Pellentesque ac lectus lectus. Nullam sed leo sed sem dictum malesuada. In non nibh a diam sollicitudin eleifend. Maecenas mi augue, tempus at imperdiet vitae, volutpat eu felis. Aenean ac auctor libero, sed semper nulla. Praesent ut facilisis libero, in vestibulum ligula. Ut id ipsum ac neque cursus aliquam. Proin volutpat libero sed est tempus imperdiet. Mauris facilisis mi a lorem laoreet convallis. Nullam sagittis massa vel leo porttitor dignissim. Ut pulvinar dolor nec augue tempor placerat. Curabitur volutpat pretium tellus, eu condimentum libero ornare lacinia.\n"
				"Cras hendrerit est eget mollis bibendum. Etiam vel metus nec nulla molestie fermentum pharetra at velit. Donec tellus risus, cursus id sodales in, convallis sit amet eros. Nunc auctor quam et accumsan dapibus. Morbi non ullamcorper purus. Phasellus nec molestie tortor, commodo aliquet quam. Etiam id pulvinar risus, a tristique tellus. Nulla consectetur lacinia odio, vitae varius augue mattis in. Pellentesque rutrum augue eget nibh pretium porta. Sed feugiat ante vel elit ultricies, eget fermentum dui blandit. Nam congue tortor ut nisl consectetur, ac ultricies lacus tempor.\n"
				"Sed nec orci at purus laoreet egestas ut ac ex. Vivamus at accumsan ante. Donec vitae ante lobortis, gravida quam quis, tempus urna. Quisque neque nulla, auctor vel risus vel, rhoncus mollis augue. Nam eu eros volutpat, semper urna non, malesuada dui. Integer congue interdum ullamcorper. Nunc semper at odio et fermentum. Praesent vehicula dui sit amet lectus rhoncus elementum. Donec fermentum lacus eu metus consequat, a dignissim tellus consectetur. Quisque euismod ligula id tellus venenatis rutrum.\n"
				"Nunc dignissim consequat risus, ut ultrices tellus dictum ut. Duis eros est, commodo non ipsum eu, tempor ornare justo. Pellentesque eu rhoncus sapien. Morbi ac tellus at nibh finibus accumsan vel et erat. Proin faucibus laoreet libero vitae dignissim. Curabitur ac enim in mauris luctus sodales. Proin lacinia quam quis sagittis lacinia. Morbi sollicitudin tellus feugiat odio varius rutrum. Donec consectetur fringilla enim sit amet semper. Vivamus ut erat at justo condimentum faucibus. Interdum et malesuada fames ac ante ipsum primis in faucibus. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Curabitur erat tellus, fermentum sed suscipit in, molestie id sapien. Praesent ultricies molestie ipsum id pulvinar. Cras blandit tempus augue a rhoncus.\n"
				"Duis egestas pellentesque eros, sit amet euismod mi sagittis quis. Maecenas pharetra risus et urna tincidunt, nec feugiat dolor commodo. Phasellus nec tempor lacus, et venenatis nibh. Suspendisse potenti. Sed convallis vel dolor sit amet pellentesque. Donec nibh massa, vestibulum quis volutpat vitae, ullamcorper in sem. Cras non ligula quam. Fusce vel quam dignissim, auctor turpis nec, semper quam. Phasellus posuere justo eu leo placerat tincidunt. Fusce sagittis odio at magna lacinia, sed lacinia sem venenatis. Praesent quam nisl, tincidunt vitae tincidunt sed, blandit a orci. Fusce vel congue diam.\n"
				"Nullam mattis ante nunc, ut porta leo auctor at. Aliquam quis quam ac felis ornare porttitor. Nulla tempus felis at ipsum porttitor, nec accumsan velit dapibus. Praesent lobortis euismod dolor, id ornare libero vehicula et. Vestibulum ac rhoncus urna, in fringilla dolor. Nunc eros tellus, mattis quis orci blandit, pharetra faucibus metus. Quisque sed mauris molestie justo laoreet tincidunt. Sed quis purus eget nisl commodo luctus. Donec convallis convallis leo, id cursus urna fringilla nec. Maecenas lacinia, lectus sit amet sodales pellentesque, sem lectus hendrerit felis, eu condimentum justo urna a mi. Curabitur odio turpis, hendrerit eu facilisis ac, tincidunt ac mi. Maecenas egestas elementum diam id vulputate. Integer et mauris quis dui ornare tristique eget at justo. Fusce et leo ut neque ornare congue. Mauris vitae faucibus turpis. Nunc nec suscipit neque.\n"
				"Nam ut orci at tortor cursus interdum sed eget eros. Donec sed commodo nibh. Cras lacinia posuere turpis dapibus porttitor. Etiam consequat mauris elit, vitae tincidunt enim mattis at. Praesent ut mi ut ligula pharetra pharetra non nec lectus. Suspendisse ut nulla eget tortor lobortis malesuada non sed ex. Sed at scelerisque ligula, eu finibus dolor. Nunc ac pulvinar lorem, a tincidunt dui. Vestibulum placerat risus ac lorem consequat efficitur. Cras commodo ex elit, et vestibulum quam suscipit ut. Praesent varius, nisi aliquet tristique vulputate, leo ipsum feugiat eros, vitae tristique metus orci sed libero. Nulla dictum, erat ac euismod bibendum, mi neque blandit odio, id condimentum libero velit eget tellus. Quisque maximus, magna sed accumsan dapibus, tellus diam faucibus mauris, nec tempus mauris lacus nec mauris. Aenean rhoncus, elit eu iaculis interdum, eros eros interdum sem, vel tempus ipsum purus sit amet orci.\n"
				"Donec lobortis pretium ex, in convallis est accumsan eget. Nunc at felis ante. Suspendisse et interdum dolor. Donec massa orci, lacinia a ligula ac, scelerisque ultrices lacus. Aliquam ullamcorper dui nec viverra condimentum. Quisque consequat sollicitudin purus tincidunt lobortis. Quisque dapibus pharetra mi at sagittis. Donec eget lacus hendrerit, lobortis urna id, mollis magna. Duis id tellus aliquet, pretium risus in, volutpat lectus. Suspendisse erat tortor, dictum ut nisi at, rutrum vulputate massa. Quisque aliquet, mauris non ullamcorper finibus, dolor mi eleifend elit, eget luctus mauris lacus ut ipsum. Proin sit amet purus nec orci vehicula congue.\n"
				"Nullam at vehicula urna. Nam porttitor tellus ipsum, a laoreet magna aliquam in. Maecenas orci dui, placerat at mauris non, sagittis suscipit nunc. Phasellus eget metus nec ante finibus placerat. Fusce posuere finibus ullamcorper. Curabitur bibendum pulvinar sapien eget porta. Suspendisse potenti.\n"
				"Proin eleifend eget est at congue. Integer posuere est justo. In imperdiet, magna sit amet ultrices condimentum, mi odio viverra nisl, nec blandit odio nunc sed ipsum. Sed justo magna, egestas nec lacus id, scelerisque faucibus sem. Maecenas lobortis libero facilisis lobortis vestibulum. Vivamus arcu leo, blandit sed consectetur non, venenatis eu leo. Phasellus pulvinar augue molestie venenatis interdum.\n"
				"Fusce interdum turpis non leo aliquet tempus. Cras et volutpat magna. Praesent eu consectetur dui. Sed congue molestie est, sed congue nisi euismod ac. Fusce non nunc eu ipsum varius convallis. Phasellus feugiat nisi nulla, eget lacinia ipsum dictum eu. Nunc pellentesque nunc non leo lacinia, sed laoreet orci malesuada. Curabitur lobortis consequat neque eget vulputate. Cras quis tempus tellus, sed tincidunt ex. In massa quam, vulputate sed accumsan mollis, porttitor sed arcu.\n"
				"Sed accumsan luctus ultrices. Cras maximus risus sed egestas sollicitudin. Fusce aliquam lacus eu tincidunt feugiat. Aliquam enim nunc, vestibulum id tortor nec, pulvinar imperdiet velit. Nam in tortor porttitor, tempor tellus in, maximus magna. Pellentesque interdum enim id est fermentum fringilla. Cras at nisi ac erat fermentum efficitur ac a lectus. Nulla tincidunt nisi vitae nisl sollicitudin tincidunt.\n"
				"Morbi iaculis nunc felis, vel pulvinar nisl dignissim lacinia. Morbi congue eros et tristique vulputate. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Vivamus sed odio eu mi faucibus bibendum ac ut dui. Ut congue metus in odio porttitor, non bibendum velit dapibus. Fusce nisi odio, ultrices imperdiet tincidunt non, facilisis ut odio. Pellentesque eu quam lacinia, auctor lectus id, facilisis lorem. Vivamus a sem nulla. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. In condimentum ultrices augue, quis ornare urna consectetur luctus.\n"
				"Nam ipsum ligula, aliquet a justo eget, lacinia dictum metus. Duis et imperdiet nibh. Vivamus pharetra dolor ut ante pulvinar consequat. Mauris interdum porttitor lectus, ut ultricies nisl suscipit lacinia. Proin commodo mauris nisi, eu scelerisque nunc viverra vel. Ut faucibus elit quis turpis varius placerat. Proin leo sapien, accumsan eget mattis vitae, sodales id sapien. Praesent imperdiet et velit vitae viverra. Nullam venenatis nibh nunc, eget malesuada nisi pellentesque non. Pellentesque id lacus placerat orci fermentum faucibus. Pellentesque vulputate, eros eget finibus bibendum, dui velit eleifend tellus, vel scelerisque purus risus at arcu. Praesent velit ex, bibendum non euismod quis, pellentesque vitae ex. Nulla facilisi. Nunc sed arcu arcu. Donec rhoncus libero vitae magna convallis dapibus.\n"
				"Mauris enim augue, cursus et massa quis, egestas aliquam dolor. Sed a blandit tortor, quis euismod nisi. Morbi pellentesque id arcu in mollis. Phasellus maximus nibh a congue consectetur. Maecenas suscipit in nulla a hendrerit. Praesent vel tempor mi. Curabitur viverra, enim id ullamcorper scelerisque, odio purus sollicitudin dolor, quis rhoncus lectus lorem laoreet sem. Nunc velit tellus, bibendum at lacus ac, mollis porta libero. Ut at ultricies enim, a tempor lorem. Sed tempus vehicula ex, id volutpat magna tincidunt non. Maecenas auctor molestie dolor, et luctus sem maximus a.\n"
				"Cras non hendrerit purus. Morbi quis lacus elit. Donec nisi urna, tempor eu tellus in, ornare semper lorem. Integer porta eu diam a sollicitudin. In ac risus sit amet lectus blandit auctor. Donec sollicitudin orci ac purus tristique, vel molestie lorem congue. Praesent suscipit odio nisl, ultricies accumsan dui viverra id. Suspendisse sed dui elit. Etiam eleifend sem non sodales sodales. Sed placerat, dolor nec consequat pulvinar, velit dui bibendum augue, sed aliquam lacus tortor auctor magna. Ut gravida vitae ipsum nec egestas. Fusce non velit sit amet erat ullamcorper tincidunt vel id leo. Sed et sem augue. Nam vitae leo egestas, maximus justo porta, cursus dolor.\n"
				"Nulla varius sagittis nibh convallis tristique. Vivamus sit amet efficitur leo. Aliquam nec dapibus leo, sodales tempor velit. Donec eu venenatis ex, nec volutpat odio. Aliquam rutrum finibus felis. Integer tincidunt ullamcorper pellentesque. Nullam ut interdum purus, nec molestie metus. Vestibulum eget ex quis orci pharetra viverra ac vitae dolor. Aenean luctus nibh sit amet mollis ultricies. Sed ornare lectus sed justo cursus, et tincidunt felis efficitur. In cursus maximus orci, vel pharetra purus feugiat at. Sed efficitur tristique magna, in pharetra velit dapibus sit amet. Integer nec dui ante. Vivamus et est massa.\n"
				"Nulla scelerisque felis vitae nisl mollis, ac faucibus risus ultrices. Vivamus vulputate leo tempus augue viverra egestas. Integer convallis hendrerit mattis. Aenean imperdiet odio sed tincidunt rutrum. Vestibulum vel ligula varius, porta metus ac, euismod nunc. Mauris viverra tellus nec neque ultricies, eget pharetra tortor egestas. Nullam ultricies quis nisl nec fringilla.\n"
				"Quisque efficitur nec velit pulvinar bibendum. Donec vel enim odio. Nam non sem ex. Nam porttitor dui eget enim egestas, vitae congue sapien malesuada. Phasellus leo enim, elementum eget gravida eget, hendrerit vitae neque. Mauris mauris erat, dapibus et nisi nec, viverra sagittis diam. Curabitur ut lectus pellentesque, venenatis odio id, pellentesque ipsum. Mauris semper tincidunt faucibus. Nulla ut ante vel turpis molestie molestie. Nunc vel nisi eu ante aliquam mollis ut vitae dolor. Curabitur a tristique augue, vitae finibus risus. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Vivamus tincidunt dignissim metus, sit amet rhoncus sapien egestas eget.\n"
				"Nullam ut leo est. Quisque consequat id dui at sollicitudin. Donec pellentesque, magna eget tristique lacinia, mi nisl aliquet lectus, dignissim euismod dolor velit at sapien. Maecenas blandit commodo hendrerit. Fusce id dui nec lorem volutpat hendrerit eget non arcu. Maecenas mattis ex eget quam venenatis, vel vulputate arcu posuere. Praesent in ut.",
				0, 20, 1920,
				Font::GetDefaultFont(), 12,
				0xFFFFFF,
				TextRenderer::TextAlign::LEFT);
	}

};

static void createCube(Entity* cube, Game& game) {
	static auto boxModel = StaticModelGeneratorBox({ 1.0f, 1.0f, 1.0f })();
	auto boxShader = game.GetAssetLoader().glsl.Load("Resources/test_shader.glsl");

	cube->AddComponent<ModelRenderComponent>(boxModel, Material({ 0.0f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));

	CosineInterpolator<float> interpolator1;
	interpolator1.AddPoint(0.0f, 0.2f);
	interpolator1.AddPoint(1.0f, 1.0f);
	interpolator1.AddPoint(2.0f, 0.2f);

	LinearInterpolator<float> interpolator2;
	interpolator2.AddPoint(0.0f, 0.0f);
	interpolator2.AddPoint(2.0f, M_PI * 2.0f);

	auto animator = cube->AddComponent<AnimatorComponent>();
	Clip clip;

	clip.AddSequence<float>([cube](float f) {
		auto modelRenderer = cube->GetComponent<ModelRenderComponent>();
		modelRenderer->SetMaterial(Material({ f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));

		cube->transform.SetTranslation({ 0.0f, f + 0.5f, 0.0f });
	}, std::move(interpolator1));

	clip.AddSequence<float>([cube](float f) {
		cube->transform.SetRotation(vec3{ 0.0f, f, 0.0f });
	}, std::move(interpolator2));

	animator->AddClip("Animation", std::move(clip));
	animator->Loop("Animation");
}

static void createRamp(Entity* ramp) {
	static auto rampModel = StaticModelGeneratorBox({ 8.0f, 1.0f, 10.0f })();

	ramp->AddComponent<ModelRenderComponent>(rampModel, Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));
	ramp->AddComponent<RigidBody>(PhysicsShape::Box({ 4.0f, 0.5f, 5.0f }));
}

static void createFloor(Entity* floor) {
	static auto floorModel = StaticModelGeneratorBox({ 40.0f, 1.0f, 40.0f })();

	floor->AddComponent<ModelRenderComponent>(floorModel, Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));
	floor->AddComponent<RigidBody>(PhysicsShape::Box({ 20.0f, 0.5f, 20.0f }));
}

static ScenePointer createScene(Game& game) {
	auto scene = game.CreateScene();
	scene->AddRootComponent<FpsUpdater>();

	auto physicsScene = scene->AddRootComponent<PhysicsScene>();
	physicsScene->SetGravity({ 0.0f, -9.81f, 0.0f });

	auto cube = scene->AddEntity(
			scene->UniqueEntityName("cube"),
			RigidTransform({ 0.0f, 0.5f, 0.0f }));
	createCube(cube, game);

	auto ramp1 = scene->AddEntity("ramp1", RigidTransform({ -8, 3, 0 }, quat(vec3{ 0, 0, -0.6f })));
	createRamp(ramp1);

	auto ramp2 = scene->AddEntity("ramp2", RigidTransform({ 8, 2, 0 }, quat(vec3{ 0, 0, 0.8f })));
	createRamp(ramp2);

	auto floor = scene->AddEntity("floor", RigidTransform({ 0, -0.5f, 0 }));
	createFloor(floor);

	auto light = scene->AddEntity("main_light");
	auto lightComponent = light->AddComponent<DirectionalLight>(Color(1.0f, 1.0f, 1.0f, 1.0f), vec3{ 0.2f, -2.0f, -1.0f });
	lightComponent->SetShadowDistance(100.0f);

	auto camera = scene->AddEntity("main_camera", RigidTransform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
	camera->AddComponent<PerspectiveCamera>("main_camera", 75.0f, 0.01f, 100.0f);
	camera->AddComponent<EulerController>();

	return scene;
}

class SandboxGame : public Game {

public:
	SandboxGame() :
			Game(GetDisplayConfiguration_(), "Sandbox") {}

	void Start() override {
		SetActiveScene(createScene(*this));

		Container ui;

		auto sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);
		sceneElement->SetGrabOnFocus(false);

		auto vignetteElement = ui.InsertElement(VignetteElement(Color{ 0.0f, 0.0f, 0.0f, 0.5f }, 0.5f, 2.0f));
		ui.AddConstraint(vignetteElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(vignetteElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		auto crosshairElement = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		auto fpsElement = ui.InsertElement(TextElement("0 FPS", 20, 0xFFFFFF));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		auto buttonElement = ui.InsertElement(ButtonElement([this](){ Stop(); }));
		ui.AddConstraint(buttonElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT,  100);
		ui.AddConstraint(buttonElement, Constraint::TOP_LEFT, buttonElement, Constraint::BOTTOM_RIGHT, -100);

		auto loremElement = ui.InsertElement(LoremElement());
		ui.AddConstraint(loremElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(loremElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		GetUI().SetContainer(std::move(ui));
		GetActiveScene()->GetRootComponent<FpsUpdater>()->SetElement(fpsElement);
	}

	static DisplayConfiguration GetDisplayConfiguration_() {
		DisplayConfiguration config;
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;
		config.shadow_quality = DisplayConfiguration::SHADOW_HIGH;
		config.aa_mode = DisplayConfiguration::AntiAliasing::MSAA_4;
		config.vsync = true;
		return config;
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)