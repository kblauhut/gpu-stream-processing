export interface Bid {
  auction: number;
  bidder: number;
  price: number;
  channel: string;
  url: string;
  date_time: number; // looks like a timestamp (ms since epoch)
  extra: string;
}

export interface Person {
  id: number;
  name: string;
  email_address: string;
  credit_card: string;
  city: string;
  state: string;
  date_time: number; // timestamp
  extra: string;
}

export interface Auction {
  id: number;
  item_name: string;
  description: string;
  initial_bid: number;
  reserve: number;
  date_time: number; // start timestamp
  expires: number; // end timestamp
  seller: number;
  category: number;
  extra: string;
}

enum EventType {
  Bid = 0,
  Person = 1,
  Auction = 2,
}

export const bidToFileLine = (bid: Bid): string => {
  return `${EventType.Bid} ${bid.auction} ${bid.bidder} ${bid.price} ${bid.channel} ${bid.url} ${bid.date_time} ${bid.extra}`;
};

export const personToFileLine = (person: Person): string => {
  return `${EventType.Person} ${person.id} ${person.name} ${person.email_address} ${person.credit_card} ${person.city} ${person.state} ${person.date_time} ${person.extra}`;
};

export const auctionToFileLine = (auction: Auction): string => {
  return `${EventType.Auction} ${auction.id} ${auction.item_name} ${auction.description} ${auction.initial_bid} ${auction.reserve} ${auction.date_time} ${auction.expires} ${auction.seller} ${auction.category} ${auction.extra}`;
};
