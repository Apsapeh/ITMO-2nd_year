plugins {
    application
    id("java")
    id("com.github.johnrengelman.shadow") version "7.0.0"
}

java {
    sourceCompatibility = JavaVersion.VERSION_17
    targetCompatibility = JavaVersion.VERSION_17
}

group = "nothing.here"

repositories {
    mavenCentral()
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))));
}

application {
    mainClass.set("nothing.here.Main") // Replace with your main class
}

tasks.shadowJar {
    archiveBaseName = "server"
    archiveClassifier = ""
}
