import { execa } from "execa";
import {
  Bid,
  Auction,
  Person,
  bidToFileLine,
  personToFileLine,
  auctionToFileLine,
} from "./events.js";
import { Command } from "commander";
import { appendFile } from "node:fs/promises";
import path from "node:path";
import { existsSync, mkdirSync, writeFileSync } from "node:fs";
import { fileURLToPath } from "node:url";
import cliProgress from "cli-progress";

const program = new Command();
program.option("-n, --num-events <number>", "Number of events to generate");
program.parse(process.argv);
const options = program.opts();

const numEvents = (options.numEvents as string | undefined) || 1000;
const progressBar = new cliProgress.SingleBar(
  {},
  cliProgress.Presets.shades_classic,
);
progressBar.start(Number(numEvents), 0);

type RawEvent = { Bid: Bid } | { Auction: Auction } | { Person: Person };

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const folderPath = path.join(__dirname, "../data");
const filePath = path.join(folderPath, "stream_data");

if (!existsSync(folderPath)) {
  mkdirSync(folderPath, { recursive: true });
}

writeFileSync(filePath, "");

const subprocess = execa("nexmark", ["-n", numEvents.toString()], {
  stdout: "pipe",
  maxBuffer: 1024 * 1024 * 2000,
});

subprocess.stdout.setEncoding("utf8");

let processedEvents = 0;
let buffer = "";
subprocess.stdout.on("data", (chunk: string) => {
  buffer += chunk;
  let lines = buffer.split("\n");
  buffer = lines.pop() || "";

  for (const line of lines) {
    if (line.trim() === "") continue;

    const obj = JSON.parse(line) as RawEvent;

    let fileLine = "";
    if ("Bid" in obj) {
      fileLine = bidToFileLine(obj.Bid);
    } else if ("Auction" in obj) {
      fileLine = auctionToFileLine(obj.Auction);
    } else if ("Person" in obj) {
      fileLine = personToFileLine(obj.Person);
    }

    if (fileLine) {
      appendFile(filePath, fileLine + "\n");
    }
  }
  processedEvents += lines.length;
  progressBar.update(processedEvents);
});

subprocess.stdout.on("end", () => {
  progressBar.stop();
});
