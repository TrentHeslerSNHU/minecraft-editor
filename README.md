# Minecraft Editor
## Description ##
This project is intended to be a free and open source editor for Minecraft: Bedrock Edition save files. It is in the early stages of development, but currently allows you to view player data from a Minecraft world, in a human-readable format.

## Roadmap ##
* ✔ - Allow users to view data values from a Minecraft: Bedrock Edition world
* ✘ - Allow users to edit data values from a Minecraft: Bedrock Edition world
* ✘ - Create a Qt GUI to simplify viewing and manipulating data values

## Credits ##
I would like to give a shoutout to the following developers, whose code served as a reference, inspiration, or dependency for this project:
* [Mojang/Microsoft](https://github.com/mojang/leveldb) - Created Minecraft and the modified version of leveldb used to store data. Without their efforts in creating this amazing game, this project wouldn't exist!
* [Google](https://github.com/google/leveldb) - Created the leveldb NoSQL database, which was modified by Mojang for use in Minecraft: Bedrock Edition. This database stores data for Minecraft saves.
* [KuanYuChang](https://gist.github.com/KuanYuChang/f768cd48e1f2e261109a1e6bfa5a07b5) - Created a simple leveldb build script that served as the basis for the build script used in this project.
* [Bedrock-OSS](https://github.com/Bedrock-OSS/bedrock-wiki/blob/wiki/docs/nbt/nbt-in-depth.md) - Created an in-depth analysis of the Minecraft: Bedrock edition NBT value storage format. This analysis proved ESSENTIAL when creating this project.
