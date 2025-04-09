<p align="center"><b>Freode</b> is a <a href="https://store.steampowered.com/app/322170/Geometry_Dash/">Geometry Dash</a> <b>mod loader</b> and <b>modding SDK</b> with a modern approach towards mod development.</p>

## Why Freode?

Unlike previous mod loaders, which merely inject the DLLs and let devs handle the rest, Freode aims to be a more comprehensive project that provides all the tools needed for creating mods in one package.

Freode's goal is to solve **mod incompatibility** - to ensure that mods work together without buttons getting misplaced or hooks mysteriously disappearing.

## "Hello World!" Example

Here's a **Hello World** mod in Freode:

```cpp
#include <Freode/Bindings.hpp>
#include <Freode/modify/MenuLayer.hpp>

using namespace freode::prelude;

class $modify(MenuLayer) {
	void onMoreGames(CCObject*) {
		FLAlertLayer::create(
			"Freode",
			"Hello World from my Custom Mod!",
			"OK"
		)->show();
	}
};
```

This code modifies what happens when the "More Games" button is clicked on the home scene in Geometry Dash, showing a custom popup.

## Documentation

Detailed documentation, tutorials, and installation instructions on using Freode can be found [here](https://docs.freode-sdk.org).

New to modding GD? Freode's documentation also comes with a handy [tutorial book](https://docs.freode-sdk.org/#/handbook/chap0) that explains all the basics of GD modding!

## Contribution

You can contribute to Freode by opening a [Pull Request](https://github.com/freode-sdk/freode/pulls)! Please follow the contribution guidelines.

## Questions, help, etc.

If you have any further questions, need help, or just want to share your love for catgirls, be sure to join [our Discord server](https://discord.gg/9e43WMKzhp)!

## License

This project is licensed under the GNU AGPL v3.0.  
Portions of this project are derived from software originally licensed under the Boost Software License v1.0, which is compatible with the AGPL.

See LICENSE and BOOST_LICENSE for details.
